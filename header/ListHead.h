
/*
    文件说明:   双链表定义头文件
    文件作者:   jsf
*/
#ifndef __DOUBLE_LIST_HEADER__
#define __DOUBLE_LIST_HEADER__

/*
	链表结构REC
*/
struct ListHead
{
	struct ListHead *pPrev;
    struct ListHead *pNext;
};

#ifdef __DEBUG__
static 
void ListHeadPrint( 
			int log,
			struct ListHead *pLh
				)
{
	printlog( log, "ListHeadPrint : %p, %p, %p\n", pLh->pPrev, pLh, pLh->pNext );
}

static
void ListHeadListPrint( 
				int log,
				struct ListHead *pLhHead 
					)
{
	struct ListHead *pLhPtr =	pLhHead->pNext;
    while ( pLhPtr != pLhHead )
    {
    	ListHeadPrint( log, pLhPtr );
        pLhPtr =	pLhPtr->pNext;
    }
}
#endif

/*
	链表根节点
*/
struct ListRoot
{
	struct ListHead *root;
};



#define LIST_HEAD_EMPTY_ROOT( root )	( (root)->root == NULL )


#ifndef offsetof
#define offsetof( TYPE, MEMBER )	( (size_t)&((TYPE *)0)->MEMBER )
#endif

#ifndef container_of
#define container_of( ptr, type, member )	({	\
    const typeof( ( ((type *)0)->member ) *__mptr =	( ptr );	\
    ( type * )( (char *)__mptr - offsetof( type, member ) );	})
#endif


/*** 确定域在结构中的字节偏移量 ***/
#define getoffset(type, name)	( (size_t)( \
			(char *)&((type *)0)->name - (char *)(type *)0 \
		 ) )
	  
/*** 根据type类型中name域的指针pointer,获得type的指针 ***/
#define gettype(type, pointer, name)	( \
		 	(type*)( (char *)((char *)pointer - getoffset(type, name)) )	\
		 )

#define GetPointer( pointer, type, name )	gettype( type, (pointer), name )

/*
	查找结构中的偏移
*/
#define GetStructOffset( type, name )	\
    ( (char *)&( ((typeof(type) *)0)->name - (char *)(typeof(type) *)0) ) )

/*
	根据结构中的成员域获取结构地址指针
*/
#define GetStructPointer( pointer, type, name )		\
    ( ( (typeof(type) *) )( (char *)( (char *)pointer - GetStructOffset( type, name ) ) ) )


////////////////////////////////////////////////////////////////////////////////
/*
	宏操作
*/

/*
	判断链表是否为空
*/
static inline
BOOL IsListHeadEmpty( const struct ListHead *pHead )
{
	assert( NULL != pHead );
	return pHead == pHead->pPrev;
}

/* 获取当前节点的后一指针 */
static inline
struct ListHead *ListHeadNextPtrGet( const struct ListHead *pNode )
{
	assert( NULL != pNode );
	return pNode->pNext;
}

/* 获取当前节点的前一指针 */
static inline
struct ListHead *ListHeadPrevPtrGet( const struct ListHead *pNode )
{
	assert( NULL != pNode );
	return pNode->pPrev;
}

/*
	获取链表的头节点
*/
static inline
struct ListHead *ListHeadRearGet( const struct ListHead *pHead )
{
	if ( !IsListHeadEmpty( pHead ) )
    {
		return ListHeadNextPtrGet( pHead );
    }
    else
    {
    	return NULL;
    }
}
/*
	获取链表的尾节点
*/
static inline
struct ListHead *ListHeadTailGet( const struct ListHead *pHead )
{
	if ( !IsListHeadEmpty( pHead ) )
    {
		return ListHeadPrevPtrGet( pHead );
    }
    else
    {
    	return NULL;
    }
}

/*
	链表节点初始化
*/
static inline
void ListHeadInitialize( struct ListHead *pListHead )
{
	assert( NULL != pListHead );
	pListHead->pPrev =	pListHead->pNext =	pListHead;
}
/*
	根据链表中的前后指针将节点插入到链表的指定位置
*/
static inline
void ListHeadInsert( struct ListHead *pPrev, 
									struct ListHead *pNext, 
									struct ListHead *pNode )
{
	assert( NULL != pPrev && NULL != pNext && NULL != pNode );

	pNode->pPrev =	pPrev;
    pNode->pNext =	pNext;
    pPrev->pNext =	pNode;
    pNext->pPrev =	pNode;
}

/*
	从节点的头部插入指定节点
*/
static inline
void ListHeadRearInsert( struct ListHead *pListHead, 
										struct ListHead *pNode )
{
	assert( NULL !=pListHead && NULL != pNode );
	ListHeadInsert( pListHead, pListHead->pNext, pNode );
}
		
/*
	从节点的尾部插入指定节点
*/
static inline
void ListHeadTailInsert( 
			struct ListHead *pListHead, 
			struct ListHead *pNode 
					)
{
	assert( NULL != pListHead );
    assert( NULL != pNode );
    
    ListHeadInsert( pListHead->pPrev, pListHead, pNode );
}

/*
	从链表的指定位置删除特定节点
*/
static inline
void ListHeadDelete( 
		struct ListHead *pPrev,
		struct ListHead *pNext 
					)
{
	assert( NULL != pPrev );
    assert( NULL != pNext );
    
	pPrev->pNext =	pNext;
    pNext->pPrev =	pPrev;
}

/*
	从节点的头部删除节点
*/
static inline
BOOL ListHeadRearDelete( 
			struct ListHead *pNode
						)
{
	BOOL ret =	FALSE;
    
    assert( NULL != pNode );

	if ( !IsListHeadEmpty( pNode ) )
    {
		ListHeadDelete( pNode, pNode->pNext->pNext );    	
        ret =	TRUE;
    }
    
    return ret;
}

/*
	从节点的尾部删除节点
*/
static inline
BOOL ListHeadTailDelete( 
			struct ListHead *pNode
						)
{
	BOOL ret =	FALSE;
	assert( NULL != pNode );

    if ( !IsListHeadEmpty( pNode ) )
    {
    	ListHeadDelete( pNode->pPrev->pPrev, pNode );
        ret =	TRUE;
    }

    return ret;
}


typedef BOOL (*pListFreeFunc )( struct ListHead * );

/*
	节点释放函数
	------------
	pListHead -	待释放的节点指针
	pFree -	节点的释放函数
*/
static inline
BOOL ListHeadFree( struct ListHead *pLhPtr, pListFreeFunc pFree )
{
	assert( NULL != pLhPtr );
    
    ListHeadDelete( pLhPtr->pPrev, pLhPtr->pNext );
    if ( NULL != pFree )
    {
    	return pFree( pLhPtr );
    }
	return FALSE;
}

/*
	节点清空函数
	----------------------------
	注意: 这里注意考虑内存释放的问题
*/

static inline
void ListHeadClear( struct ListHead *pListHead, pListFreeFunc pFree )
{
	assert( NULL != pListHead );

//	printlog( REC, "ListHeadClear : head=%p\n", pListHead );
    
	while ( !IsListHeadEmpty( pListHead ) )
    {
    	struct ListHead *pNode =	ListHeadRearGet( pListHead );
//		taskDelay( sysClkRateGet() );
        ListHeadFree( pNode, pFree );
//		printlog( LOG, "ListHeadClear cycle\n" );
//        ListHeadListPrint( pListHead );
    }
}

/*
	将一个链表中的数据转移至另外一个链表
	------------------------------------
	pDesList -	目的链表
	pSrcList -	源链表
*/
static inline
void ListHeadMove( struct ListHead *pDesList, struct ListHead *pSrcList )
{
	assert( NULL != pDesList );
    assert( NULL != pSrcList );

    if ( !IsListHeadEmpty( pSrcList ) )
    {
        struct ListHead *pPrev =	ListHeadPrevPtrGet( pSrcList );
        struct ListHead *pNext =	ListHeadNextPtrGet( pSrcList );

        ListHeadInsert( pPrev, pNext, pDesList );

		ListHeadInitialize( pSrcList );
    }
}

#endif

