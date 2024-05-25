/*
 * collector.h
 */

#ifndef COLLECTOR_H
#define COLLECTOR_H

void mark_sweep_gc(HeapBase* hb);

void mark_compact_gc(HeapBase* hb);

void copy_collection_gc(HeapBase* hb);

//void generational_gc(List*roots, ...);

#endif
