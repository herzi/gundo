/* This file is part of gundo, a multilevel undo/redo facility for GTK+
 * 
 * AUTHORS
 *	Sven Herzberg		<herzi@gnome-de.org>
 *
 * Copyright (C) 2005		Sven Herzberg
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#ifndef GOBJECT_HELPERS_H
#define GOBJECT_HELPERS_H

#define G_DEFINE_IFACE(TypeName, type_name, parent) \
GType \
type_name##_get_type(void) { \
	static GType type = 0; \
	\
	if(!type) { \
		static const GTypeInfo info = { \
			sizeof(TypeName##Iface) \
		}; \
		\
		type = g_type_register_static(parent, #TypeName, &info, 0); \
	} \
	\
	return type; \
}

#endif /* GOBJECT_HELPERS_H */

