/*-
 * Copyright (c) 2005-2006 Benedikt Meurer <benny@xfce.org>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#if !defined (EXO_INSIDE_EXO_H) && !defined (EXO_COMPILATION)
#error "Only <exo/exo.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef __EXO_UTILS_H__
#define __EXO_UTILS_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

void                    exo_noop        (void) G_GNUC_PURE;
gint                    exo_noop_one    (void) G_GNUC_PURE;
gint                    exo_noop_zero   (void) G_GNUC_PURE;
gpointer                exo_noop_null   (void) G_GNUC_PURE;
gboolean                exo_noop_true   (void) G_GNUC_PURE;
gboolean                exo_noop_false  (void) G_GNUC_PURE;

#if !GTK_CHECK_VERSION (3, 0, 0)

/* inline function implementations */
#if (defined(G_CAN_INLINE) && defined(__GNUC__) && defined(__i386__) && defined(__OPTIMIZE__)) || defined(__EXO_UTILS_C__)

G_INLINE_FUNC void      exo_atomic_inc  (gint *value);
G_INLINE_FUNC gboolean  exo_atomic_dec  (gint *value);

/**
 * exo_atomic_inc:
 * @value : a pointer to the integer value to increment.
 *
 * Increments the integer at @value by one in an
 * atomic fashion.  The advantage over using
 * g_atomic_int_inc() is that calls to this
 * function can be inlined.
 *
 * Since : 0.3.1
 **/
G_INLINE_FUNC void
exo_atomic_inc (gint *value)
{
#if defined(__GNUC__) && defined(__i386__) && defined(__OPTIMIZE__)
  __asm__ __volatile__ ("lock; incl %0"
                        : "=m" (*value)
                        : "m" (*value));
#else
  g_atomic_int_inc (value);
#endif
}

/**
 * exo_atomic_dec:
 * @value : a pointer to the integer value to decrement.
 *
 * Decrements the integer at @value by one in an atomic
 * fashion and returns %TRUE if the @value dropped to
 * zero by this operation, else %FALSE. The advantage
 * over using g_atomic_int_dec() is that calls to this
 * function can be inlined.
 *
 * Returns: %TRUE if @value dropped to zero, else %FALSE.
 *
 * Since : 0.3.1
 **/
G_INLINE_FUNC gboolean
exo_atomic_dec (gint *value)
{
#if defined(__GNUC__) && defined(__i386__) && defined(__OPTIMIZE__)
  gint result;

  __asm__ __volatile__ ("lock; xaddl %0,%1"
                        : "=r" (result), "=m" (*value)
                        : "0" (-1), "m" (*value));

  return (result == 1);
#else
  return g_atomic_int_dec_and_test (value);
#endif
}

#else /* (G_CAN_INLINE && __GNUC__ && __i386__ && __OPTIMIZE__) || __EXO_UTILS_C__ */

/* We do not use the inline versions of the exo_atomic_* functions, unless we can
 * benefit from it. If the compiler is not able to inline it (or won't inline it
 * because of any policy) it'd actually cause more overhead than calling the
 * g_atomic_int_* counterparts directly.
 */
#define exo_atomic_inc(value) G_STMT_START{ g_atomic_int_inc ((value)); }G_STMT_END
#define exo_atomic_dec(value) (g_atomic_int_dec_and_test ((value)))

#endif /* (G_CAN_INLINE && __GNUC__ && __i386__ && __OPTIMIZE__) || __EXO_UTILS_C__ */

#endif

G_END_DECLS

#endif /* !__EXO_UTILS_H__ */
