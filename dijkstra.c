#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	MAX_CITY_LEN	8
#define INF 999

typedef struct destNode{

	struct destNode *next;
	char destName[MAX_CITY_LEN+1];
	int edge;
}destNode;

typedef struct destNodeList{
	destNode *header;
}destNodeList;

typedef struct srcNode{

	char srcName[MAX_CITY_LEN+1];
	struct srcNode *next;
	destNodeList *destList;
	int dist;
}srcNode;

typedef struct srcNodeList{
	srcNode *header;
	int size;
}srcNodeList;

typedef struct Node{
	struct Node *next;
	char nodeName[MAX_CITY_LEN+1];
	int dist;
	struct Node *prev;
}Node;

typedef struct NodeList{
	Node *header;
	int size;
}NodeList;


void removeSrcNode(char *srcName, srcNodeList *srcList){
	srcNode *pTmp1 = srcList->header;
	srcNode *pTmp2 = NULL;
	while(pTmp1 != NULL){
		if(strcmp(pTmp1->srcName, srcName) == 0){
			break;
		}
		pTmp2 = pTmp1;
		pTmp1 = pTmp1->next;
	}
	if(pTmp1 == srcList->header){
		srcList->header = pTmp1->next;
	}else{
		pTmp2->next = pTmp1->next;
		}
}

destNodeList* getDestList(char *srcName, srcNodeList *srcList){
	srcNode *pTmp1 = srcList->header;
	while(pTmp1->next != NULL){
		if(strcmp(pTmp1->srcName, srcName) == 0){
			break;
		}
		pTmp1 = pTmp1->next;
	}
	return pTmp1->destList;
}


destNodeList*  insertSrcNode(char *srcName, srcNodeList *srcList){
	srcNode *pNewNode = (srcNode *)malloc(sizeof(srcNode));
	pNewNode->dist = INF;
	destNodeList *newDestList = (destNodeList *)malloc(sizeof(destNodeList));
	pNewNode->destList = newDestList;
	strcpy(pNewNode->srcName, srcName);
	
	if(srcList->header == NULL){
		srcList->header = pNewNode;
		pNewNode->next = NULL;
		srcList->size = srcList->size+1;
		return pNewNode->destList;
	}

	srcNode *tmp2 = srcList->header;
	srcNode *tmp1 = NULL;

	while(tmp2->next != NULL){
		tmp1 = tmp2;
		tmp2 = tmp2->next;
		if(strcmp(pNewNode->srcName, tmp2->srcName) < 0){
			tmp1->next = pNewNode;
			pNewNode->next = tmp2;
			srcList->size = srcList->size+1;
			return pNewNode->destList;
		}
		else if(strcmp(pNewNode->srcName, tmp2->srcName) == 0){
			free(pNewNode);
			return tmp2->destList;
		}
	}
	tmp2->next = pNewNode;
	pNewNode->next = NULL;
	srcList->size = srcList->size+1;
	return pNewNode->destList;
}


void insertDestNode(char *destName, int edge ,destNodeList *destList){
	destNode *pNewNode = (destNode *)malloc(sizeof(destNode));
	pNewNode->edge = edge;
	strcpy(pNewNode->destName, destName);

	if(destList->header == NULL){
		destList->header = pNewNode;
		pNewNode->next = NULL;
		return;
	}

	destNode *tmp2 = destList->header;
	destNode *tmp1 = NULL;

	while(tmp2->next != NULL){
		tmp1 = tmp2;
		tmp2 = tmp2->next;
		if(strcmp(pNewNode->destName, tmp2->destName) < 0){
			tmp1->next = pNewNode;
			pNewNode->next = tmp2;
			return;
		}
		else if(strcmp(pNewNode->destName, tmp2->destName) == 0 ){
			if(tmp2->edge == pNewNode->edge){
				free(pNewNode);
				return;
			}else{
				while(tmp1->edge == tmp2->edge){
					if(pNewNode->edge < tmp2->edge){
						break;
					}
					tmp1 = tmp2;
					tmp2 = tmp2->next;
				}
				tmp1->next = pNewNode;
				pNewNode->next = tmp2;
				return;
			}
		}
	}
	tmp2->next = pNewNode;
	pNewNode->next = NULL;
	return;
}

//추후에 이부분에 대해서 priority Queue나 heap을 적용해 볼것. 
srcNode* extractMin(srcNodeList *srcList){
	srcNode *pTmp = srcList->header;
	srcNode *min = pTmp;
	while(pTmp->next != NULL){
		if(min->dist > pTmp->dist){
			min = pTmp;
		}
		pTmp= pTmp->next;
	}
	return min;
}

srcNode* getSrcNode(char *srcName, srcNodeList *srcList){
	srcNode *pSrcTmp = srcList->header;
	while(pSrcTmp != NULL){
		if(strcmp(pSrcTmp->srcName, srcName) == 0) {
			return pSrcTmp;
		}
		pSrcTmp = pSrcTmp->next;
	}
	return NULL;
}


void setPrev(char * nodeName, char *prevNodeName ,int dist, NodeList* distList){
	Node *pPrev = distList->header;
	Node *pNode = distList->header;

	while(pPrev != NULL){
		if(strcmp(pPrev->nodeName, prevNodeName) == 0){
			break;
		}
		pPrev = pPrev->next;
	}
	while(pNode != NULL){
		if(strcmp(pNode->nodeName, nodeName) == 0){
			break;
		}
		pNode  = pNode->next;
	}
	
	pNode->dist = dist;
	pNode->prev = pPrev;

}
/*
*/

void initDistList(NodeList *distList, srcNodeList *srcList, char* srcInput){

	Node *distHeader = (Node *)malloc(sizeof(Node));
	distHeader->dist = 0;
	distHeader->next = NULL;
	distHeader->prev = NULL;
	strcpy(distHeader->nodeName, srcInput);
	distList->header = distHeader;
	//DistList의 header 초기화.
	
	srcNode *pSrcNode = srcList->header;
	Node *pNode = distList->header;
	while(pSrcNode != NULL){
		if(strcmp(srcInput, pSrcNode->srcName) == 0){
			pSrcNode = pSrcNode->next;
			continue;
		}
		Node *pNewNode = (Node *)malloc(sizeof(Node));
		pNewNode->dist = 0;
		pNewNode->prev = NULL;
		pNewNode->next = NULL;
		strcpy(pNewNode->nodeName, pSrcNode->srcName);
		pNode->next = pNewNode;
		distList->size = ( distList->size ) + 1;
		pNode = pNode->next;
		pSrcNode = pSrcNode->next;
	}
	
}

NodeList* dijkstra(srcNodeList *srcList, NodeList *distList ,char *srcInput){
	//출발 vertex의 거리 값을 초기화 해주느 부분.
	srcNode *pSrcTmp = NULL;
	pSrcTmp = getSrcNode(srcInput, srcList);	//출발지 srcNode를 받아옴.
	pSrcTmp->dist = 0;
	
	while(srcList->header != NULL){
		//아직 방문하지 않은 vertex 들 중에서 dist값이 가장 작은 vertex를 선택한다!
		pSrcTmp = extractMin(srcList);
		destNode *pDestTmp = getDestList(pSrcTmp->srcName, srcList)-> header;
		srcNode *V = NULL;
		srcNode *U = NULL;
		while(pDestTmp != NULL){
			V = getSrcNode(pDestTmp->destName, srcList);
			U = getSrcNode(pSrcTmp->srcName, srcList);
			if(V == NULL){
			//이미 방분한 vertex일 경우, continue;
				pDestTmp = pDestTmp->next;
				continue;
			}
			if(V->dist  > U->dist  + pDestTmp->edge){
				V->dist =  U->dist + pDestTmp->edge;
				setPrev(pDestTmp->destName, pSrcTmp->srcName, pDestTmp->edge, distList);
			}
			pDestTmp = pDestTmp->next;
		}
		removeSrcNode(pSrcTmp->srcName, srcList);
	}
	return distList;

}


void main(int argc, char* argv[]){

	if(argc != 2){
		printf("잘못된 input file : input file을1개  입력해주세요.\n");
		exit(1);
	}
	char srcInput[MAX_CITY_LEN+1]= {'\0'};
	char destInput[MAX_CITY_LEN+1] = {'\0'};
	FILE *rFile = NULL;
	char src[MAX_CITY_LEN+1] = {'\0'};
	char dest[MAX_CITY_LEN+1] = {'\0'};
	int edge = 0;
	NodeList *resultList = NULL;
	Node *pDest = NULL;
	Node *pDist = NULL;
	int i = 0;
	int count = 0;
	int distSum = 0;

	//srcNodeList가 만들어짐.
	srcNodeList *srcList = (srcNodeList *)malloc(sizeof(srcNodeList));
	srcList->header = NULL;
	srcList->size = 0;
	destNodeList *tmpDestList = NULL;

	//fileRead 시작!
	if((rFile = fopen(argv[1], "r")) != NULL){
		
		while(fscanf(rFile, "%s %s %d", src, dest, &edge) != EOF){
			//printf("this is for test (%s %s %d)\n", src, dest, edge);

			//printf("begein : insertSrcNode(%s)\n", src);
			tmpDestList = insertSrcNode(src, srcList);

			//printf("begein : insertDestNode(%s)\n", dest);
			insertDestNode(dest, edge, tmpDestList);

			//printf("begein : insertSrcNode(%s)\n", dest);
			tmpDestList = insertSrcNode(dest, srcList);

			//printf("begein : insertDestNode(%s)\n", src);
			insertDestNode(src, edge, tmpDestList);

			//printf("end loop\n");
		}

	}

	printf("출발점을 입력해 주세요. (글자수 8이하) \n");
	scanf("%s", srcInput);
	printf("도착점을 입력해 주세요. (글자수 8이하) \n");
	scanf("%s", destInput);
	
	NodeList *distList = (NodeList *)malloc(sizeof(NodeList));
	initDistList(distList, srcList, srcInput);
	
	resultList = dijkstra(srcList, distList,srcInput);
	
	char distNodeNameArray[distList->size][MAX_CITY_LEN+1];	//출력을 위한 이차원 배열.
	int distArray[distList->size];
	
	pDest = resultList->header;

	printf("%s 부터 %s 까지의 최단 경로 : ", srcInput, destInput);

	while(pDest != NULL){
		if(strcmp(pDest->nodeName,destInput) == 0 ){
			break;
		}
		pDest = pDest->next;
	}

	pDist = pDest;
	
	while(pDist != NULL){
		strcpy(distNodeNameArray[i], pDist->nodeName);
		distArray[i++] = pDist->dist;
		count++;		//나중에 출력을 위해 쓰인다.
		distSum = distSum + pDist->dist;
		pDist = pDist->prev;
	}
	printf("%s", distNodeNameArray[count-1]);
	for(i=count-2; i>= 0; i--){
		printf("-(%d)->%s", distArray[i], distNodeNameArray[i]);
	}
	
	printf("\n최단 경로 비용 : %d \n", distSum);	
}

