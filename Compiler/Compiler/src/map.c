#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

HashMap * createNewHashMap(int size)
{
	HashMap *newMap = malloc(sizeof(HashMap));
	if (newMap == NULL) {
		return NULL;
	}

	newMap->elementCount = 0;
	newMap->size = size;
	newMap->list = calloc(size, sizeof(MapNode *));
	if (newMap->list == NULL) {
		return NULL;
	}

	return newMap;
}

MapNode * createNewMapNode(char * key, int stackOffset)
{
	MapNode * newNode = malloc(sizeof(MapNode));
	if (newNode == NULL) {
		return NULL;
	}

	newNode->key = malloc(strlen(key) + 1 * sizeof(char));
	if (key == NULL) {
		free(newNode);
		return NULL;
	}

	strncpy(newNode->key, key, strlen(key) + 1);
	newNode->stackOffset = stackOffset;
	newNode->next = NULL;
	return newNode;
}

int expandMap(HashMap * map, int newSize)
{
	map->list = realloc(map->list, sizeof(MapNode *) * newSize);
	if (map->list == NULL) {
		return -1;
	}

	for (int i = map->elementCount; i < map->size; i++) {
		map->list[i] = NULL;
	}

	map->size = newSize;

	return 0;
}

int hashKey(int size, char * key)
{
	int sum = 0;
	char *temp = key;
	while (*temp++) {
		sum += *temp;
	}
	return sum % size;
}

int lookupNode(HashMap * map, char * key)
{
	int pos = hashKey(map->size, key);
	int value = -1;
	MapNode *node = map->list[pos];
	MapNode *temp = node;
	while (temp) {
		if (strcmp(temp->key, key) == 0) {
			value = temp->stackOffset;
		}
		temp = temp->next;
	}
	return value;
}

int insertHashMap(HashMap * map, char * key, int stackOffset)
{
	MapNode *nodeToInsert = createNewMapNode(key, stackOffset);

	if (map->elementCount >= map->size) {
		if (expandMap(map, map->size * 2) != 0) {
			return -1;
		}
	}

	
	int pos = hashKey(map->size, key);
	MapNode *node = map->list[pos];
	if (node) {
		nodeToInsert->next = node;
	}
	map->list[pos] = nodeToInsert;
	return 0;
}

void freeHashMap(HashMap * map)
{
	for (int i = 0; i < map->size; i++) {
		MapNode *temp = map->list[i];
		freeNodeList(temp);
	}
	free(map->list);
	free(map);
}

void freeNodeList(MapNode * node)
{
	if (node) {
		freeNodeList(node->next);
		free(node->key);
		free(node);
	}
}
