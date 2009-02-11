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

#include "gundo-tool-undo.h"

#include <gtk/gtkarrow.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtkstock.h>

G_DEFINE_TYPE(GundoToolUndo, gundo_tool_undo, GTK_TYPE_TOOL_ITEM)

GtkToolItem*
gundo_tool_undo_new(void) {
	return g_object_new(GUNDO_TYPE_TOOL_UNDO, NULL);
}

void
gundo_tool_undo_connect(GundoToolUndo* self, GundoSequence* sequence) {
	if(self->sequence) {
		g_object_unref(self->sequence);
#warning "gundo_tool_undo_connect(): FIXME: disconnect from unused sequences"
		self->sequence = NULL;
	}

	if(sequence) {
		self->sequence = g_object_ref(sequence);
		gundo_make_undo_sensitive(GTK_WIDGET(self), self->sequence);
	} else {
		gtk_widget_set_sensitive(GTK_WIDGET(self), FALSE);
	}
}

static void
gundo_tool_undo_class_init(GundoToolUndoClass* self) {
#warning "gundo_tool_undo_class_init(): FIXME: listen to the toolbar_reconfigured signal"
#warning "gundo_tool_undo_class_init(): FIXME: disconnect from the sequence on delete"
}

static void
gtu_icon_clicked(GundoToolUndo* self, GtkWidget* icon) {
	gundo_sequence_undo(self->sequence);
}

static void
gtu_icon_raise(GtkWidget* arrow, GtkButton* icon) {
	gtk_button_set_relief(icon, GTK_RELIEF_NORMAL);
}

static void
gtu_icon_sink(GtkWidget* arrow, GtkButton* icon) {
	gtk_button_set_relief(icon, GTK_RELIEF_NONE);
}

static void
gtu_toggle_list(GundoToolUndo* self, GtkToggleButton* arrow) {
	static GtkWidget* popup = NULL;
	
	if(!popup) {
		GtkWidget* frame;
		popup = gtk_window_new(GTK_WINDOW_POPUP);
		frame = gtk_frame_new(NULL);
		gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
		gtk_container_add(GTK_CONTAINER(popup), frame);
		gtk_container_add(GTK_CONTAINER(frame), gtk_label_new("This is the undo list popup"));
		gtk_widget_show_all(frame);
	}
	
	if(gtk_toggle_button_get_active(arrow)) {
		gtk_widget_show(popup);
	} else {
		gtk_widget_hide(popup);
	}
}

static void
gundo_tool_undo_init(GundoToolUndo* self) {
	GtkWidget* hbox = gtk_hbox_new(FALSE, 0);

	self->icon_button = gtk_button_new_from_stock(GTK_STOCK_UNDO);
	gtu_icon_sink(NULL, GTK_BUTTON(self->icon_button));
	g_signal_connect_swapped(self->icon_button, "clicked",
				 G_CALLBACK(gtu_icon_clicked), self);
	gtk_box_pack_start(GTK_BOX(hbox), self->icon_button, FALSE, FALSE, 0);

	self->arrow_button = gtk_toggle_button_new();
	gtk_button_set_relief(GTK_BUTTON(self->arrow_button), GTK_RELIEF_NONE);
	g_signal_connect_after(self->arrow_button, "enter",
			       G_CALLBACK(gtu_icon_raise), self->icon_button);
	g_signal_connect_after(self->arrow_button, "leave",
			       G_CALLBACK(gtu_icon_sink), self->icon_button);
	g_signal_connect_swapped(self->arrow_button, "toggled",
				 G_CALLBACK(gtu_toggle_list), self);
	gtk_container_add(GTK_CONTAINER(self->arrow_button), gtk_arrow_new(GTK_ARROW_DOWN, GTK_SHADOW_IN));
	gtk_box_pack_start(GTK_BOX(hbox), self->arrow_button, FALSE, FALSE, 0);

	gtk_widget_show_all(hbox);
	gtk_container_add(GTK_CONTAINER(self), hbox);
}

