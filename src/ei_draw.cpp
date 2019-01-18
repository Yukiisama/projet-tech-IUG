#include "ei_draw.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

namespace ei {

linked_point_t arc(const Point& center, float radius, int start_angle, int end_angle)
{
    linked_point_t list;
    float angle = float(end_angle - start_angle) * M_PI/180.f;
    int nbpts = radius * fabs(angle);
    for(int i=0; i<=nbpts; i++){
        Point p = Point(round(radius * cosf(start_angle * M_PI/180.f + (float)i * angle / (float)nbpts) + center.x()),
                        round(radius * sinf(start_angle * M_PI/180.f + (float)i * angle / (float)nbpts) + center.y()));
        list.push_back(p);
    }
    return list;
}

linked_point_t rounded_frame(const Rect& rect, float radius, bt_part part)
{
    Point pt = rect.top_left;
    linked_point_t list;
    list.clear();
    if(part != BT_BOTTOM){
        pt.x() += radius;
        pt.y() += (rect.size.height() - radius);
        list.splice(list.end(), arc(pt, radius, 135, 180));
        pt.y() -= (rect.size.height() - 2.f * radius);
        list.splice(list.end(), arc(pt, radius, 180, 270));
        pt.x() += (rect.size.width() - 2.f * radius);
        list.splice(list.end(), arc(pt, radius, 270, 315));
    }

    if(part == BT_TOP) {
        list.push_back(Point(rect.top_left.x() + 2*rect.size.width() / 3, rect.top_left.y() + rect.size.height() / 2));
        list.push_back(Point(rect.top_left.x() + rect.size.width() / 3, rect.top_left.y() + rect.size.height() / 2));
        return list;
    }

    if(part == BT_BOTTOM){
        pt.x() += rect.size.width() - radius;
        pt.y() += radius;
        list.splice(list.end(), arc(pt, radius, 315, 360));
    }else{
        list.splice(list.end(), arc(pt, radius, 315, 360));
    }

    pt.y() += (rect.size.height() - 2.f * radius);
    list.splice(list.end(), arc(pt, radius, 0, 90));
    pt.x() -= (rect.size.width() - 2.f * radius);
    list.splice(list.end(), arc(pt, radius, 90, 135));

    if(part == BT_BOTTOM){
        list.push_back(Point(rect.top_left.x() + rect.size.width() / 3, rect.top_left.y() + rect.size.height() / 2));
        list.push_back(Point(rect.top_left.x() + 2*rect.size.width() / 3, rect.top_left.y() + rect.size.height() / 2));
    }
    return list;
}

void draw_line(surface_t surface, const Point& start,
                  const Point& end, const color_t& color,
                  const Rect* clipper)
{
    al_set_target_bitmap((ALLEGRO_BITMAP*) surface);
    if(clipper)
        al_set_clipping_rectangle(clipper->top_left.x(), clipper->top_left.y(), clipper->size.width(), clipper->size.height());
    al_draw_line(start.x(), start.y(), end.x(), end.y(), al_map_rgba(color.red, color.green, color.blue, color.alpha), 1);
}

void draw_polyline(surface_t surface,
                      const linked_point_t& point_list,
                      const color_t color, const Rect* clipper)
{
    if(point_list.size() < 2)
        return;

    Point start = point_list.front();
    auto it = point_list.begin();
    it++;
    while(it!=point_list.end()) {
        draw_line(surface, start, *it, color, clipper);
        start = *it;
        it++;
    }
}

static inline color_t alpha_blend(const color_t in_pixel, const color_t dst_pixel)
{
    color_t blended;
    float alpha = ((float)in_pixel.alpha) / 255.f;
    blended.red   = (1.f -  alpha) * dst_pixel.red   + alpha * in_pixel.red;
    blended.green = (1.f -  alpha) * dst_pixel.green + alpha * in_pixel.green;
    blended.blue  = (1.f -  alpha) * dst_pixel.blue  + alpha * in_pixel.blue;
    blended.alpha = 255;
    return blended;
}

typedef struct edge_t {
    int y_max;      //< Max ordinate
    int x_min;     //< Min abscissa
    int dx;         //< Displacement along x axis
    int dy;         //< Displacement along y axis
    int stepx;       //< Step along y axis (+/-1)
    int fraction;
    struct edge_t* next;
} edge_t;

/**
 * \brief   Compute min and max scanline (in the y direction) for the given polygon
 *
 * @param   point_list      A linked list of the points of the polygon.
 * @param   min_scanline    Stores the minimum scanline
 * @param   min_scanline    Stores the maximum scanline
 */
void min_max_scanline(const linked_point_t& point_list, int& min_scanline, int& max_scanline)
{
    min_scanline = point_list.front().y();
    max_scanline = min_scanline;

    for(const Point& point : point_list) {
        if (point.y() < min_scanline)
            min_scanline = point.y();
        if (point.y() > max_scanline)
            max_scanline = point.y();
    }
}

/**
 * \brief   Allocate and store all edges of the polygon into an edge_table (edge_t)
 *          in the increasing order of y and x.
 *          For example, edge_table[current_scanline] contains all edges for which
 *          lowest y position is current_scanline + min_scanline
 *
 * @param   point_list      A linked list of the points of the polygon.
 * @param   min_scanline    The minimum scanline
 * @param   max_scanline    The maximum scanline
 */
edge_t** build_edge_table(const linked_point_t& point_list, int min_scanline, int max_scanline)
{
    // Allocate and initialize edge table
    edge_t** edge_table = new edge_t*[max_scanline - min_scanline + 1];
    for (int i = 0; i <= (max_scanline - min_scanline); i++)
        edge_table[i] = NULL;

    // Fill edge table
    bool_t all_processed = EI_FALSE;
    edge_t* current_edge, *tmp_edge;
    int current_scanline;

    Point end, start = point_list.front();
    auto it = point_list.begin();
    it++;

    while (!all_processed) {
        if (it == point_list.end()) {
            end = point_list.front();
            all_processed = EI_TRUE;
        }else{
            end = *it;
        }

        // skip horizontal edges
        if (start.y() != end.y()) {
            current_edge = new edge_t();
            if (start.y() < end.y()) {
                current_scanline = start.y();
                current_edge->y_max = end.y();
                current_edge->x_min = start.x();
                current_edge->dx = end.x() - start.x();
                current_edge->dy = end.y() - start.y();
            } else {
                current_scanline = end.y();
                current_edge->y_max = start.y();
                current_edge->x_min = end.x();
                current_edge->dx = start.x() - end.x();
                current_edge->dy = start.y() - end.y();
            }
            if (current_edge->dx < 0) {
                current_edge->dx = -current_edge->dx;
                current_edge->stepx = -1;
            } else {
                current_edge->stepx = 1;
            }
            if (current_edge->dx > current_edge->dy) {
                current_edge->fraction = current_edge->dy - current_edge->dx;
            } else {
                current_edge->fraction = current_edge->dx - current_edge->dy;
            }
            current_edge->dx = (current_edge->dx << 1);
            current_edge->dy = (current_edge->dy << 1);
            current_scanline -= min_scanline;

            // Insert in ET sorted by increasing y and x of the lower end
            if (edge_table[current_scanline] == NULL) {
                edge_table[current_scanline] = current_edge;
                current_edge->next = NULL;
            } else {
                tmp_edge = edge_table[current_scanline];
                while (tmp_edge->next != NULL && (tmp_edge->next->y_max <= current_edge->y_max)) {
                    if ((tmp_edge->next->y_max == current_edge->y_max) && (tmp_edge->next->x_min > current_edge->x_min))
                        break;
                    tmp_edge = tmp_edge->next;
                }
                current_edge->next = tmp_edge->next;
                tmp_edge->next = current_edge;
            }
        }
        // Process next edge
        start = end;
        it++;
    }

    return edge_table;
}


/**
 * @brief Debugging functions to print the edge tables
 */
void print_edge_table_entry(edge_t* edge)
{
    while (edge != NULL) {
        printf("[%d, %d, %d, %d, %d, %d] ", edge->y_max,
               edge->x_min, edge->dx, edge->dy,
               edge->stepx, edge->fraction);
        edge = edge->next;
    }
    printf("\n");
}

void print_edge_table(edge_t** edge_table, int min_scanline, int max_scanline)
{
    edge_t* current_edge;
    printf("Edge table:\n");
    for (int i = 0; i <= (max_scanline - min_scanline); i++) {
        if (edge_table[i] != NULL) {
            printf("%d: ", i + min_scanline);
            current_edge = edge_table[i];
            print_edge_table_entry(current_edge);
        }
    }
    printf("\n");
}

void draw_polygon(surface_t surface, const linked_point_t &point_list,
                  const color_t& color, const Rect* clipper)
{
    edge_t* active_edge_table = NULL;
    edge_t* current_edge, * tmp_edge, * prev_edge;
    Point pos;
    int current_scanline;
    color_t p;

    if (point_list.empty()) {
        fprintf(stderr, "no point for the polygon\n");
        return;
    }

    hw_surface_lock(surface);

    // Compute min/max scanline
    int min_scanline, max_scanline;
    min_max_scanline(point_list, min_scanline, max_scanline);


    edge_t** edge_table = build_edge_table(point_list, min_scanline, max_scanline);

#ifdef DEBUG
    print_edge_table(edge_table, min_scanline, max_scanline);
#endif

    for (current_scanline = 0; current_scanline <= (max_scanline - min_scanline); current_scanline++) {
        // Move edges from ET[current_scanline] to AET sorted by increasing x of the lower end
        current_edge = edge_table[current_scanline];
        prev_edge = active_edge_table;
        while (current_edge != NULL) {
            if (active_edge_table == NULL) {
                active_edge_table = current_edge;
                break;
            } else {
                if (active_edge_table->x_min > current_edge->x_min) {
                    tmp_edge = current_edge->next;
                    current_edge->next = active_edge_table;
                    active_edge_table = current_edge;
                    current_edge = tmp_edge;
                } else {
                    while (prev_edge->next != NULL &&
                           prev_edge->next->x_min < current_edge->x_min) {
                        prev_edge = prev_edge->next;
                    }
                    tmp_edge = current_edge->next;
                    current_edge->next = prev_edge->next;
                    prev_edge->next = current_edge;
                    current_edge = tmp_edge;
                }
            }
        }

        // Remove from AET edges for wich y_max = current_scanline + min_scanline
        current_edge = active_edge_table;
        tmp_edge = NULL;
        while (current_edge != NULL) {
            if (current_edge->y_max == current_scanline + min_scanline) {
                if (tmp_edge == NULL)
                    active_edge_table = current_edge->next;
                else
                    tmp_edge->next = current_edge->next;
            } else {
                tmp_edge = current_edge;
            }
            current_edge = current_edge->next;
        }

#ifdef DEBUG
        printf("%d: ", current_scanline + min_scanline);
        print_edge_table_entry(active_edge_table);
#endif

        // Fill pixel values
        current_edge = active_edge_table;
        while (current_edge != NULL) {
            pos.y() = current_scanline + min_scanline;
            for (pos.x() = ceil(current_edge->x_min); pos.x() <= floor(current_edge->next->x_min); pos.x()++) {
                if (clipper == NULL ||
                        ((clipper->top_left.x() <= pos.x()) && (pos.x() < clipper->top_left.x() + clipper->size.width())
                         && (clipper->top_left.y() <= pos.y()) && (pos.y() < clipper->top_left.y() + clipper->size.height()))) {
                    p = hw_get_pixel(surface, pos);
                    hw_put_pixel(surface, pos, alpha_blend(color, p));
                }
            }
            current_edge = current_edge->next->next;
        }

        current_edge = active_edge_table;
        tmp_edge = NULL;
        prev_edge = NULL;
        int i = 0;
        while (current_edge != NULL) {
            // Update next x_ymin using Bersenham
            if (current_edge->dx > current_edge->dy) {
                while (current_edge->fraction < 0) {
                    current_edge->x_min += current_edge->stepx;
                    current_edge->fraction += current_edge->dy;
                }
                current_edge->fraction -= current_edge->dx;
            } else {
                if (current_edge->fraction >= 0) {
                    current_edge->x_min += current_edge->stepx;
                    current_edge->fraction -= current_edge->dy;
                }
                current_edge->fraction += current_edge->dx;
            }

            // Make sure TCA remains sorted by increasing x of the lower end
            if ((tmp_edge != NULL) && (tmp_edge->x_min > current_edge->x_min)) {
                // swap entries
                if (prev_edge != NULL) {
                    prev_edge->next = current_edge;
                    tmp_edge->next = current_edge->next;
                    current_edge->next = tmp_edge;
                } else {
                    active_edge_table = current_edge;
                    tmp_edge->next = current_edge->next;
                    current_edge->next = tmp_edge;
                }
                current_edge = tmp_edge;
            }
            prev_edge = tmp_edge;
            tmp_edge = current_edge;
            current_edge = current_edge->next;
            i++;
        }
    }
    hw_surface_unlock(surface);

    for (int i = 0; i <= (max_scanline - min_scanline); i++) {
        if (edge_table[i] != NULL) {
            delete edge_table[i];
        }
    }
    delete[] edge_table;
}


void draw_text(surface_t surface, const Point* where,
                  const char* text, const font_t font,
                  const color_t* color)
{
    if (text == NULL || color == NULL){
        fprintf(stderr, "no text or color specified");
        return;
    }

    surface_t s_text;

    if (font == NULL) {
        s_text = hw_text_create_surface(text, default_font, color);
    } else {
        s_text = hw_text_create_surface(text, font, color);
    }

    ei_copy_surface(surface, s_text, where, EI_TRUE);

    hw_surface_free(s_text);
}

void fill(surface_t surface, const color_t* color, const bool_t use_alpha)
{
    const color_t* c = color == NULL ? &font_default_color : color;

    al_set_target_bitmap((ALLEGRO_BITMAP*) surface);
    if(use_alpha == EI_TRUE)
        al_clear_to_color(al_map_rgba(c->red, c->green, c->blue, c->alpha));
    else
        al_clear_to_color(al_map_rgba(c->red, c->green, c->blue, 0xff));
}


void ei_copy_surface(surface_t destination, const surface_t source,
                     const Point* where, const bool_t use_alpha)
{
    if(use_alpha == EI_TRUE)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
    else
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ZERO);
    al_set_target_bitmap((ALLEGRO_BITMAP*) destination);
    al_draw_bitmap((ALLEGRO_BITMAP*) source, where->x(), where->y(), 0);
}

}
