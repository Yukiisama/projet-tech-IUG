#include "ei_draw.h"
#include "ei_application.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
using namespace std;
namespace ei {

linked_point_t arc(const Point& center, float radius, int start_angle, int end_angle)
{
    linked_point_t list;
    float angle = float(end_angle - start_angle) * M_PI/180.f;
    int nbpts = radius * fabs(angle);
    for(int i=0; i<=nbpts; i++){
        if(radius>0.f){
            Point p = Point(round(radius * cosf(start_angle * M_PI / 180.f + (float)i * angle / (float)nbpts) + center.x()),
                            round(radius * sinf(start_angle * M_PI / 180.f + (float)i * angle / (float)nbpts) + center.y()));
            list.push_back(p);
        }else{
            list.push_back(center);
        }
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
    int cx, cy, cw, ch;
    al_get_clipping_rectangle(&cx, &cy, &cw, &ch);
    if(clipper)
        al_set_clipping_rectangle(clipper->top_left.x(), clipper->top_left.y(), clipper->size.width(), clipper->size.height());
    al_draw_line(start.x(), start.y(), end.x(), end.y(), al_map_rgba(color.red, color.green, color.blue, color.alpha), 1);
    al_set_clipping_rectangle(cx, cy, cw, ch);
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

void draw_arrow(surface_t surface, const Point where, int dimension, Rect* clipper){
    int size = dimension/3;
    Point diagonal_start = Point(where.x(),where.y()+size);
    Point diagonal_end = Point(where.x()+((dimension-size)/2+0.5),where.y()+((dimension-size)/2+0.5+size));

    color_t color = {0,0,0,150};

    for(;diagonal_start.y()>where.y();){
        draw_line(surface,diagonal_start,diagonal_end,color,clipper);
        diagonal_start.x()+=1;diagonal_end.x()+=1;
        draw_line(surface,diagonal_start,diagonal_end,color,clipper);
        diagonal_start.y()-=1;diagonal_end.y()-=1;
    }

    Point start = Point(where.x(),where.y()+dimension);
    Point end = Point(where.x()+dimension,where.y()+dimension);

    for(;start.x()<end.x();start.x()+=1,start.y()-=1,end.y()-=1){
        draw_line(surface,start,end,color,clipper);
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

/**
 * @brief convert_linked_point_to_vertices
 * @param vertex_count  Number of vectices in the end of the function.
 * @param point_list    A linked list of the points of the polygon.
 * @param clipper       If not NULL, the drawing is restricted within this rectangle.
 * @return vertices - Interleaved array of (x, y) vertex coordinates
 */
float* convert_linked_point_to_vertices(int * vertex_count,linked_point_t point_list,const Rect* clipper){
    int size = point_list.size()*2;
    float * vertices=new float[size];
    int index=0;
    for(linked_point_t::reverse_iterator it=point_list.rbegin(); it!=point_list.rend();++it){
            vertices[index*2]=it->x();
            vertices[index*2+1]=it->y();
            index++;
    }
    *vertex_count=index;
    return vertices;

}

/**
 * @brief draw_button   Where to draw the text. The surface must be *locked* by \ref hw_surface_lock.
 * @param surface       Where to draw the text. The surface must be *locked* by \ref hw_surface_lock.
 * @param rect          Rectangle which contains the button's topleft and it's size.
 * @param color         The color of the button.
 * @param radius        Radius used to round the button.
 * @param border_width  Border of the button.
 * @param clipper       If not NULL, the drawing is restricted within this rectangle.
 * @param relief        Relief of the button.
 */
void draw_button(surface_t surface, Rect *rect, const color_t color, int radius,int border_width, const Rect *clipper,relief_t relief)
{
    al_set_target_bitmap((ALLEGRO_BITMAP*) surface);
    color_t tint;
    tint.red = color.red + (0.25 * (255-color.red));
    tint.green = color.green + (0.25 * (255 - color.green));
    tint.blue = color.blue + (0.25 * (255 - color.blue));
    tint.alpha =255;
    color_t shade;
    shade.red = color.red *0.25;
    shade.green = color.green * 0.25;
    shade.blue = color.blue * 0.25;
    shade.alpha = 255;
    Rect new_rec = Application::getInstance()->intersectedRect(*rect,*clipper);
    if(relief == ei_relief_sunken){
        int vertex_count=0;
        const float *vertices  = convert_linked_point_to_vertices(&vertex_count,rounded_frame(new_rec, radius, BT_BOTTOM),clipper);
        al_draw_filled_polygon(vertices,vertex_count,al_map_rgba(tint.red, tint.green, tint.blue,tint.alpha));

        int vertex_count2=0;
        const float *vertices2  = convert_linked_point_to_vertices(&vertex_count2,rounded_frame(new_rec, radius, BT_TOP),clipper);
        al_draw_filled_polygon(vertices2,vertex_count2,al_map_rgba(shade.red, shade.green, shade.blue,shade.alpha));
    }else
    {
        int vertex_count=0;
        const float *vertices  = convert_linked_point_to_vertices(&vertex_count,rounded_frame(new_rec, radius, BT_TOP),clipper);
        al_draw_filled_polygon(vertices,vertex_count,al_map_rgba(tint.red, tint.green, tint.blue,tint.alpha));
        int vertex_count2=0;
        const float *vertices2  = convert_linked_point_to_vertices(&vertex_count2,rounded_frame(new_rec, radius, BT_BOTTOM),clipper);
        al_draw_filled_polygon(vertices2,vertex_count2,al_map_rgba(shade.red, shade.green, shade.blue,shade.alpha));
    }
    Rect *inner_rect = rect;
    float x1;
    float y1;
    float x2;
    float y2;
    if(clipper){
         x1 = max(inner_rect->top_left.x(),clipper->top_left.x());
         y1 = max(inner_rect->top_left.y(),clipper->top_left.y());
         x2 = min(inner_rect->top_left.x()+inner_rect->size.width(),clipper->top_left.x()+clipper->size.width());
         y2 = min(inner_rect->top_left.y()+inner_rect->size.height(),clipper->top_left.y()+clipper->size.height());
    }
    else{
        x1 = inner_rect->top_left.x();
        y1 = inner_rect->top_left.y();
        x2 = inner_rect->top_left.x()+inner_rect->size.width();
        y2 = inner_rect->top_left.y()+inner_rect->size.height();
    }
    x1+=border_width;
    y1+=border_width;
    x2-=border_width;
    y2-=border_width;

    al_draw_filled_rounded_rectangle(x1,y1,x2,y2,radius,radius,al_map_rgba(color.red, color.green, color.blue,color.alpha));
    
}

/**
 * @brief draw_polygon_pick_surface  use al_draw_filled_polygon to draw polygone on surface
 * @param surface   surface to draw
 * @param vertices  Interleaved array of (x, y) vertex coordinates
 * @param vertex    Number of vertices in the array
 * @param color     Color of the filled polygon
 */
void draw_polygon_pick_surface(surface_t surface,const float* vertices,int vertex,const color_t color){
    al_set_target_bitmap((ALLEGRO_BITMAP*) surface);
    al_draw_filled_polygon(vertices,vertex,al_map_rgba(color.red, color.green, color.blue,color.alpha));
}

/**
 * \brief Draws text by calling \ref hw_text_create_surface.
 *
 * @param surface   Where to draw the text. The surface must be *locked* by \ref hw_surface_lock.
 * @param where     Coordinates, in the surface, where to anchor the *top-left corner of the rendered text.
 * @param text      The string of the text. Can't be NULL.
 * @param font      The font used to render the text. If NULL, the \ref ei_default_font is used.
 * @param color     The text color. Can't be NULL. The alpha parameter is not used.
 */
void draw_text(surface_t surface, const Point* where,
                  const char* text, const font_t font,
                  const color_t* color,Rect* clipper)
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

    color_t p;
    color_t d;
    color_t f;
    Point src_pos=Point(0,0);
    Point dst_pos;
    src_pos.x()=0;src_pos.y()=0;
    hw_surface_lock(surface);
    hw_surface_lock(s_text);
    Size size =hw_surface_get_size(s_text);
    for(int y = where->y();y<where->y()+size.height();y++){
        for(int x= where->x();x<where->x()+size.width();x++){
            if(clipper==NULL || ((clipper->top_left.x()<=x) &&(x<clipper->top_left.x()+clipper->size.width())
                                 && (clipper->top_left.y()<=y) &&(y<clipper->top_left.y() + clipper->size.height()))){
                p=hw_get_pixel(s_text,src_pos);
                dst_pos.x()=x;dst_pos.y()=y;
                d=hw_get_pixel(surface,dst_pos);
                f=alpha_blend(p,d);
                hw_put_pixel(surface,dst_pos,f);
                src_pos.x()=src_pos.x()+1;
            }
        }
        src_pos.y()=src_pos.y()+1;
        src_pos.x()=0;
    }
    hw_surface_unlock(surface);
    hw_surface_unlock(s_text);

    hw_surface_free(s_text);
}
/**
 * @brief draw_rectangle    Draw filled rectangle on the surface.
 * @param surface           Where to draw the text. The surface must be *locked* by \ref hw_surface_lock.
 * @param r                 The rectangle postion and size.
 * @param color             The text color. Can't be NULL. The alpha parameter is not used.
 * @param clipper           If not NULL, the drawing is restricted within this rectangle.
 */
void draw_rectangle(surface_t surface, Rect r,const color_t color,  Rect * clipper){
    //if(!Application::getInstance()->isIntersect(r,*clipper)) return;
    al_set_target_bitmap((ALLEGRO_BITMAP*) surface);
    float x1=0;
    float y1=0;
    float x2=0;
    float y2=0;
    if(clipper){
         x1 = max(r.top_left.x(),clipper->top_left.x());
         y1 = max(r.top_left.y(),clipper->top_left.y());
         x2 = min(r.top_left.x()+r.size.width(),clipper->top_left.x()+clipper->size.width());
         y2 = min(r.top_left.y()+r.size.height(),clipper->top_left.y()+clipper->size.height());
    }
    else{
        x1 = r.top_left.x();
        y1 = r.top_left.y();
        x2 = r.top_left.x()+r.size.width();
        y2 = r.top_left.y()+r.size.height();
    }
    al_draw_filled_rectangle( x1,  y1,  x2,  y2, al_map_rgba(color.red, color.green, color.blue,color.alpha));
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
