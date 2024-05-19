/*
 * collector.h
 */

#ifndef COLLECTOR_H
#define COLLECTOR_H

void mark_sweep_gc(Heap* hb);

void mark_compact_gc(Heap* hb);

void copy_collection_gc(Heap* hb);

//void generational_gc(List*roots, ...);

#endif
