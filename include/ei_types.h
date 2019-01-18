/**
 * @file  ei_types.h
 *
 * @brief   Type, constant, and global definitions for the ei library.
 *
 *  Created by François Bérard on 18.12.11.
 *  Modified by Pierre Bénard and Aurélie Bugeau
 *
 */

#ifndef EI_TYPES_H
#define EI_TYPES_H

#include <stdint.h>
#include <vector>
#include <list>
#include <string>

namespace ei {

/********** Basic. **********/

/**
 * @brief The boolean type used in the library.
 */
typedef enum { EI_FALSE = 0, EI_TRUE = 1 } bool_t;

/********** Tags. **********/

/**
 * @brief A string that can be attached to a widget.
 *        All widgets have the tag of the name of their widget class, and the tag "all".
 */
typedef std::string tag_t;

/**
 * @brief linked list of tags
 */
typedef std::list<tag_t> linked_tag_t;

/********** Drawing. **********/

/**
 * @brief generic 2D vector class
 */
template<class T> class Vector {
public:
    Vector(T x=0, T y=0) {
        value[0] = x;
        value[1] = y;
    }

    virtual ~Vector() {}

    Vector(const Vector& o) {
        value[0] = o.value[0];
        value[1] = o.value[1];
    }

    T x() const { return value[0]; }
    T y() const { return value[1]; }

    T& x() { return value[0]; }
    T& y() { return value[1]; }

    T width() const { return value[0]; }
    T height() const { return value[1]; }

    T& width() { return value[0]; }
    T& height() { return value[1]; }

    operator Vector<int>() const { return Vector<int>(value[0],value[1]); }
    operator Vector<float>() const { return Vector<float>(value[0],value[1]); }

    Vector<T>& operator=(const Vector<T> &p) {
        value[0] = p.value[0];
        value[1] = p.value[1];
        return *this;
    }

    Vector<T>& operator+=(const Vector<T> &p) {
        value[0] += p.value[0];
        value[1] += p.value[1];
        return *this;
    }

    Vector<T>& operator-=(const Vector<T> &p) {
        value[0] -= p.value[0];
        value[1] -= p.value[1];
        return *this;
    }

    Vector<T>& operator*=(const Vector<T> &p) {
        value[0] *= p.value[0];
        value[1] *= p.value[1];
        return *this;
    }

    const Vector<T> operator+(const Vector<T> &p) const {
        return Vector<T>(*this) += p;
    }

    const Vector<T> operator-(const Vector<T> &p) const {
        return Vector<T>(*this) -= p;
    }

    const Vector<T> operator*(const Vector<T> &p) const {
        return Vector<T>(*this) -= p;
    }

    bool operator==(const Vector<T> &p) const {
        return value[0] == p.value[0] &&  value[1] = p.value[1];
    }

    bool operator!=(const Vector<T> &p) const {
        return !(*this == p);
    }

protected:
    T value[2];
};

/**
 * @brief A 2D point with integer coordinates.
 */
typedef Vector<int> Point;

/**
 * @brief A 2D size with floating point dimensions.
 */
typedef Vector<float> Size;


/**
 * @brief A rectangle defined by its top-left corner, and its size.
 */
class Rect {
public:
    Point top_left;  ///< Coordinates of the top-left corner of the rectangle.
    Size size;       ///< Size of the rectangle.
    Rect(Point top_left = Point(), Size size = Size())
        : top_left(top_left), size(size) {}
};

/**
 * @brief linked list of rectangles.
 */
typedef std::list<Rect> linked_rect_t;

/**
 * @brief linked list of points.
 */
typedef std::list<Point> linked_point_t;

/**
 * @brief A color with transparency.
 *
 *    Each channel is represented as an 8 bits unsigned interger,
 *    hence channel's minimum value is 0, maximum is 255.
 */
typedef struct {
    unsigned char red;    ///< The red component of the color.
    unsigned char green;  ///< The green component of the color.
    unsigned char blue;   ///< The blue component of the color.
    unsigned char alpha;  ///< The transparency of the color. 0 is invisible,
    ///  255 is totally opaque.
} color_t;

/**
 * @brief The default background color of widgets.
 */
static const color_t default_background_color = {0xA0, 0xA0, 0xA0, 0xff};

/**
 * @brief Identifies one particular point of a rectangle.
 */
typedef enum {
    ei_anc_none = 0,   ///< No anchor defined.
    ei_anc_center,     ///< Anchor in the center.
    ei_anc_north,      ///< Anchor on the top side, centered horizontally.
    ei_anc_northeast,  ///< Anchor on the top-right corner.
    ei_anc_east,       ///< Anchor on the right side, centered vertically.
    ei_anc_southeast,  ///< Anchor on the bottom-right corner.
    ei_anc_south,      ///< Anchor on the bottom side, centered horizontally.
    ei_anc_southwest,  ///< Anchor on the bottom-left corner.
    ei_anc_west,       ///< Anchor on the left side, centered vertically.
    ei_anc_northwest   ///< Anchor on the top-left corner.
} anchor_t;

/**
 * @brief Type of relief.
 */
typedef enum {
    ei_relief_none = 0,  ///< No relief (i.e. flat).
    ei_relief_raised,    ///< Above the screen.
    ei_relief_sunken     ///< Inside the screen.
} relief_t;

/**
 * @brief Set of axis.
 */
typedef enum {
    ei_axis_none = 0,  ///< No axis.
    ei_axis_x,         ///< Horizontal axis.
    ei_axis_y,         ///< Vertical axis.
    ei_axis_both       ///< Both horizontal and vertical axis.
} axis_set_t;

/********** Fonts management. **********/

/**
 * @brief An opaque type for handling fonts.
 *
 *    Fonts are created by calling \ref hw_text_font_create and released
 *    by calling \ref hw_text_font_free.
 */
typedef void* font_t;

/**
 * @brief The default font used in widgets.
 */
static font_t default_font = nullptr;

static const int font_default_size = 22;  ///< Default font color
static const color_t font_default_color = {0x00, 0x00, 0x00, 0xff};
static const char default_font_filename[] = DATA_DIR"font.ttf";

/********** Banner management. **********/
static const color_t default_banner_color = {0x63, 0x69, 0x70, 0xff};

static const int default_button_border_width  = 4;    ///< The default border width of button widgets.
static const int default_button_corner_radius = 10;   ///< The default corner radius of button widgets.

}

#endif
