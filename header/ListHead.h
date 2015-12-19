
/*
    �ļ�˵��:   ˫������ͷ�ļ�
    �ļ�����:   jsf
*/
#ifndef __DOUBLE_LIST_HEADER__
#define __DOUBLE_LIST_HEADER__

/*
	����ṹREC
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
	������ڵ�
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


/*** ȷ�����ڽṹ�е��ֽ�ƫ���� ***/
#define getoffset(type, name)	( (size_t)( \
			(char *)&((type *)0)->name - (char *)(type *)0 \
		 ) )
	  
/*** ����type������name���ָ��pointer,���type��ָ�� ***/
#define gettype(type, pointer, name)	( \
		 	(type*)( (char *)((char *)pointer - getoffset(type, name)) )	\
		 )

#define GetPointer( pointer, type, name )	gettype( type, (pointer), name )

/*
	���ҽṹ�е�ƫ��
*/
#define GetStructOffset( type, name )	\
    ( (char *)&( ((typeof(type) *)0)->name - (char *)(typeof(type) *)0) ) )

/*
	���ݽṹ�еĳ�Ա���ȡ�ṹ��ַָ��
*/
#define GetStructPointer( pointer, type, name )		\
    ( ( (typeof(type) *) )( (char *)( (char *)pointer - GetStructOffset( type, name ) ) ) )


////////////////////////////////////////////////////////////////////////////////
/*
	�����
*/

/*
	�ж������Ƿ�Ϊ��
*/
static inline
BOOL IsListHeadEmpty( const struct ListHead *pHead )
{
	assert( NULL != pHead );
	return pHead == pHead->pPrev;
}

/* ��ȡ��ǰ�ڵ�ĺ�һָ�� */
static inline
struct ListHead *ListHeadNextPtrGet( const struct ListHead *pNode )
{
	assert( NULL != pNode );
	return pNode->pNext;
}

/* ��ȡ��ǰ�ڵ��ǰһָ�� */
static inline
struct ListHead *ListHeadPrevPtrGet( const struct ListHead *pNode )
{
	assert( NULL != pNode );
	return pNode->pPrev;
}

/*
	��ȡ�����ͷ�ڵ�
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
	��ȡ�����β�ڵ�
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
	����ڵ��ʼ��
*/
static inline
void ListHeadInitialize( struct ListHead *pListHead )
{
	assert( NULL != pListHead );
	pListHead->pPrev =	pListHead->pNext =	pListHead;
}
/*
	���������е�ǰ��ָ�뽫�ڵ���뵽�����ָ��λ��
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
	�ӽڵ��ͷ������ָ���ڵ�
*/
static inline
void ListHeadRearInsert( struct ListHead *pListHead, 
										struct ListHead *pNode )
{
	assert( NULL !=pListHead && NULL != pNode );
	ListHeadInsert( pListHead, pListHead->pNext, pNode );
}
		
/*
	�ӽڵ��β������ָ���ڵ�
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
	�������ָ��λ��ɾ���ض��ڵ�
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
	�ӽڵ��ͷ��ɾ���ڵ�
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
	�ӽڵ��β��ɾ���ڵ�
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
	�ڵ��ͷź���
	------------
	pListHead -	���ͷŵĽڵ�ָ��
	pFree -	�ڵ���ͷź���
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
	�ڵ���պ���
	----------------------------
	ע��: ����ע�⿼���ڴ��ͷŵ�����
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
	��һ�������е�����ת��������һ������
	------------------------------------
	pDesList -	Ŀ������
	pSrcList -	Դ����
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

