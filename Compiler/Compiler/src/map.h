#ifndef MAP_H
#define MAP_H

typedef struct MapNode {
	char *key;
	int stackOffset;

	struct MapNode* next;
} MapNode;


typedef struct HashMap {
	int elementCount;
	int size;
	MapNode **list;
} HashMap;


HashMap* createNewHashMap(int size);
MapNode *createNewMapNode(char *key, int stackOffset);
int expandMap(HashMap *map, int newSize);
int hashKey(int size, char *key);
int lookupNode(HashMap *map, char *key);
int insertHashMap(HashMap* map, char *key, int stackOffset);
void freeHashMap(HashMap *map);
void freeNodeList(MapNode *node);

#endif