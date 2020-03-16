   //   #include "binomial.h" // those statements are unnecessary i already add those libraries into my code
     //  #include "fatal.h"   // but just in case i will push them also 
       #include <math.h>
	          typedef double ElementType;
       // #define Infinity (6000000000L)
		#define MAX 200
        #ifndef _BinHeap_H
		char chArr[100] ;
		int  intArr[100] ;
		#define MaxTrees (12)   /* Stores 2^9 -1 items */
        #define Capacity (4095)
        struct BinNode;
        typedef struct BinNode *BinTree;
        struct Collection;
        typedef struct Collection *BinQueue;
        double totalwtTime = 0 ;

        BinQueue Initialize( void );
        void Destroy( BinQueue H );
        BinQueue MakeEmpty( BinQueue H );
        BinQueue Insert(char prName[1000], double execNum, double execStPt, double wtTime , double priorVal , double started, BinQueue H ) ;
        double DeleteMin( BinQueue H );
        BinQueue Merge( BinQueue H1, BinQueue H2 );
        double FindMin( int time ,  BinQueue H  , double eMax);
        double FindMin4execPt (BinQueue H) ;
		int IsEmpty( BinQueue H );
        int IsFull( BinQueue H );
        #endif
/* END */
/* START: fig6_52.txt */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )
        typedef struct BinNode *Position;

        struct BinNode
        {
		   // ElementType Item;
            char prName[1000] ;
			double  execNum ;
			double  execStPt ;
			double  wtTime ;
			double  priorVal ;	
			double  started ;
			int index ;
			struct BinNode *term_next ;
            Position    LeftChild;
            Position    NextSibling;
        };		
        struct Collection
        {
            int CurrentSize;
            BinTree TheTrees[ MaxTrees ];
        };

        BinQueue
        Initialize( void )
        {
            BinQueue H;
            int i;

            H = malloc( sizeof( struct Collection ) );
            if( H == NULL )
                FatalError( "Out of space!!!" );
            H->CurrentSize = 0;
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            return H;
        }

        static void
        DestroyTree( BinTree T )
        {
            if( T != NULL )
            {
                DestroyTree( T->LeftChild );
                DestroyTree( T->NextSibling );
                free( T );
            }
        }

        void
        Destroy( BinQueue H )
        {
            int i;

            for( i = 0; i < MaxTrees; i++ )
                DestroyTree( H->TheTrees[ i ] );
        }

        BinQueue
        MakeEmpty( BinQueue H )
        {
            int i;

            Destroy( H );
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            H->CurrentSize = 0;

            return H;
        }
		
		
		
			
        /* Not optimized for O(1) amortized performance */
        BinQueue
        Insert( char prName[1000], double execNum, double execStPt, double wtTime , double priorVal , double started, BinQueue H )
        {
            BinTree NewNode;
            BinQueue OneItem;
			int i;
		
            NewNode = malloc( sizeof( struct BinNode ) );
            if( NewNode == NULL )
                FatalError( "Out of space!!!" );
            NewNode->LeftChild = NewNode->NextSibling = NULL;
			NewNode->execNum = execNum ;
			NewNode->execStPt = execStPt ;
			NewNode->wtTime = wtTime ;
			NewNode->priorVal = priorVal ;
			NewNode->started = started ;
            OneItem = Initialize( );
            OneItem->CurrentSize = 1;
            OneItem->TheTrees[ 0 ] = NewNode;
            return Merge( H, OneItem );
        }		
	
		void addNodeToHeap(struct BinNode **head , char prName[1000], double execNum, double execStPt, double wtTime , double priorVal , double started , int index) {		
		struct BinNode *newNode = (struct BinNode*)malloc(sizeof(struct BinNode)) ;
		strcpy(newNode->prName , prName) ;
		newNode->execNum = execNum ;
		newNode->execStPt = execStPt ;
    	newNode->wtTime = wtTime ;
		newNode->priorVal = priorVal ;
		newNode->started = started ;
		newNode->index = index ;
		newNode->term_next = NULL ;
		struct BinNode *temp = *head ;
			if(*head == NULL) {
				*head = newNode ;	
			}	
			
			else {
				while(temp->term_next != NULL) {
				temp = temp->term_next ;
				}	
				temp->term_next = newNode ;
			}
		} 
			double priorValCalc (double execNum , double eMax) { // this function simply finds the priority value for corresponding BinNode
		double x = 2 * execNum;
		double y = 3 * eMax;
		double result = -pow(x/y,3) ;
		double result1 = execNum*(1/exp(result)) ;
		return result1 ;
		
		}
		
								void heapDelete(struct BinNode**listP, double value)  // if theres no executable process at that exact time on the  processor ; simply remove corresponding node .
				{	
				  struct BinNode  *currP, *prevP;
				  prevP = NULL;
				  for (currP = *listP;
					currP != NULL;
					prevP = currP, currP = currP->term_next) {			
				    if (currP->execStPt == value) { 
				      if (prevP == NULL) {
				        *listP = currP->term_next;
				      } else {
				        prevP->term_next = currP->term_next;
				      }
				      free(currP);

				      return;
				    }
				  }
				}
							
				double find_minimum(struct BinNode**temp ,double time , double eMax  , int q ){ // this function's duty is finding waiting times to corresponding processes
				struct BinNode *testPtr = *temp ;
				int testVar = 0 ;
				for(testPtr ; testPtr != NULL ; testPtr = testPtr->term_next) {
					testVar++ ;
				}	
				struct BinNode *ptr = *temp ;
				struct BinNode *ptrX = *temp ;	
				struct BinNode *ptrY = *temp ;
				int min_execNum = 0 ;
				double n = 0 , m = 0;
			double min_value=ptr->priorVal;
				int execstpt = ptr->execStPt ;
		    while(ptr!=NULL){
		      if(min_value > ptr->priorVal && time >= ptr->execStPt){
		  	  	min_value=ptr->priorVal;		
			  	execstpt = ptr->execStPt ;
			  }
				ptr = ptr->term_next ;
			}
			for(ptrX ; ptrX != NULL ; ptrX = ptrX->term_next) {
				if(time >= ptrX->execStPt && ptrX->execStPt != execstpt) {
				ptrX->wtTime++ ;	
				}
			}
			for(ptrY ; ptrY != NULL ; ptrY = ptrY->term_next) {
					if(time >= ptrY->execStPt && ptrY->execStPt == execstpt) {									
								 if(q >= ptrY->started) {
							        	ptrY->wtTime = time - ptrY->execStPt ;
									 	n = ptrY->execStPt ;	
										min_execNum = ptrY->execNum ;
									    totalwtTime += ptrY->wtTime ;
										printf("%s\t   %0.f\n" , ptrY->prName , ptrY->wtTime) ;
										heapDelete(temp , n) ; // if there is no executionable process ; then simply remove it
										return min_execNum ;				
								}		
								else if ( q < ptrY->started) {
									if((ptrY->execNum - 1) >= 1 && testVar != 1 ) {
										ptrY->execNum--;
										m = priorValCalc(ptrY->execNum , eMax) ;	
										ptrY->priorVal = m ;
									}	
									else if ((ptrY->execNum - 1) < 1) {
										n = ptrY->execStPt ;
										totalwtTime += ptrY->wtTime ;
										
										printf("%s\t   %0.f\n" , ptrY->prName , ptrY->wtTime) ;												
										heapDelete(temp , n) ;
									return ;		
									}   
									else if (testVar == 1) {
										ptrY->wtTime = time - ptrY->execStPt ;
										n = ptrY->execStPt ;
										totalwtTime += ptrY->wtTime ;
										printf("%s\t   %0.f\n" , ptrY->prName , ptrY->wtTime) ;
										heapDelete(temp , n) ;
									return ;	
									}
								}
								
						
					}
						
			
			
			
			}		  
		return min_execNum ;
		}
/* START: fig6_56.txt */
        double
        DeleteMin( BinQueue H )
        {
            int i, j;
            int MinTree;   /* The tree with the minimum item */
            BinQueue DeletedQueue;
            Position DeletedTree, OldRoot;
            double MinItem;
	        if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem= MAX;
				
			//  for (i=0; i < max_act_const; i++) {
			//    MinItem.from[i].s=-1;
			//    MinItem->mn.from[i].a=m.from[i].a;
			//  }
                return MinItem;
            }

            MinItem = MAX;
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                    H->TheTrees[ i ]->priorVal < MinItem )
                {
                    /* Update minimum */
                    MinItem = H->TheTrees[ i ]->priorVal;
					
                    MinTree = i;
                }
            }

            DeletedTree = H->TheTrees[ MinTree ];
            OldRoot = DeletedTree;
            DeletedTree = DeletedTree->LeftChild;
            free( OldRoot );

            DeletedQueue = Initialize( );
            DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
            for( j = MinTree - 1; j >= 0; j-- )
            {
                DeletedQueue->TheTrees[ j ] = DeletedTree;
                DeletedTree = DeletedTree->NextSibling;
                DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
            }

            H->TheTrees[ MinTree ] = NULL;
            H->CurrentSize -= DeletedQueue->CurrentSize + 1;

            Merge( H, DeletedQueue );
            return MinItem;
        }
/* END */	
		
        double
        FindMin(int time  , BinQueue H , double eMax )
        {
            int i;
            double MinItem;
		//	printf("%d" , eMax) ;
            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem=0;
                return MinItem;
            }

            MinItem = MAX;
			
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                            H->TheTrees[ i ]->priorVal < MinItem  && time >= H->TheTrees[i]->execStPt) {
                    MinItem = H->TheTrees[ i ]->priorVal;
					printf("%d\n"  , MinItem) ;					
						if((H->TheTrees[ i ]->execNum - 1) >= 1) {
							H->TheTrees[ i ]->execNum-- ;
							H->TheTrees[ i ]->priorVal = priorValCalc(	H->TheTrees[ i ]->execNum , eMax) ;					
						}
						else if ((H->TheTrees[ i ]->execNum - 1) >= 1) {
							
						}
				}	
            }

            return MinItem;
        }
		
		
		
		
		
		
		double
        FindMin4execPt(BinQueue H )
        {
            int i;
            double MinItem;

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem=0;
                return MinItem;
            }

            MinItem = MAX;
			
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                            H->TheTrees[ i ]->execStPt < MinItem ) {
                    MinItem = H->TheTrees[ i ]->execStPt;
				}	
            }

            return MinItem;
        }
			
        int
        IsEmpty( BinQueue H )
        {
            return H->CurrentSize == 0;
        }

        int IsFull( BinQueue H )
        {
            return H->CurrentSize == Capacity;
        }

/* START: fig6_54.txt */
        /* Return the result of merging equal-sized T1 and T2 */
        BinTree
        CombineTrees( BinTree T1, BinTree T2 )
        {
            if( T1->priorVal > T2->priorVal )
                return CombineTrees( T2, T1 );
            T2->NextSibling = T1->LeftChild;
            T1->LeftChild = T2;
            return T1;
        }
/* END */

/* START: fig6_55.txt */
        /* Merge two binomial queues */
        /* Not optimized for early termination */
        /* H1 contains merged result */

        BinQueue
        Merge( BinQueue H1, BinQueue H2 )
        {
            BinTree T1, T2, Carry = NULL;
            int i, j;

            if( H1->CurrentSize + H2->CurrentSize > Capacity )
                Error( "Merge would exceed capacity" );

            H1->CurrentSize += H2->CurrentSize;
            for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
            {
                T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];

                switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
                {
                    case 0: /* No trees */
                    case 1: /* Only H1 */
                        break;
                    case 2: /* Only H2 */
                        H1->TheTrees[ i ] = T2;
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 4: /* Only Carry */
                        H1->TheTrees[ i ] = Carry;
                        Carry = NULL;
                        break;
                    case 3: /* H1 and H2 */
                        Carry = CombineTrees( T1, T2 );
                        H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                        break;
                    case 5: /* H1 and Carry */
                        Carry = CombineTrees( T1, Carry );
                        H1->TheTrees[ i ] = NULL;
                        break;
                    case 6: /* H2 and Carry */
                        Carry = CombineTrees( T2, Carry );
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 7: /* All three */
                        H1->TheTrees[ i ] = Carry;
                        Carry = CombineTrees( T1, T2 );
                        H2->TheTrees[ i ] = NULL;
                        break;
                }
            }
            return H1;
        }
		
		// by Borahan Tümer
		BinTree printTree(BinTree p, BinTree *r, int i)
		{
		  BinTree t[20]={NULL}, q; int j;
		  for ( j=0; j<i; j++ ) t[j]= r[j];
		  i=0;
		  if (p!=NULL) { 
		    printf("& %2.1lf ",p->priorVal);
			q=p->NextSibling;
			j=0;
			do {
			  while (q!=NULL) { 
			    printf("%2.1lf ",q->priorVal); 
			    if (q->LeftChild != NULL) { r[i]=q->LeftChild; i++; }
			    q=q->NextSibling;
			  }
			  q=t[j++];
			} while (q!=NULL);
		  }
		  else return NULL;
		  //for (j=0; j<i; j++) t[j]=NULL;
		  printf("\n");
		  printTree(p->LeftChild, r, i);
		}
		
				
		//	/*
		main()
		{
		BinQueue H1, H2;
		  BinTree p, r[20]={NULL};
		  ElementType Item;
		  char ch;
		 // int i;
		  H1 = Initialize( );
		struct BinNode *ptr1 = NULL ;
		struct BinNode *ptr2 = NULL ;
		struct BinNode *node = NULL ;
		FILE *fp = fopen("input.txt" , "r") ;
		if(fp == NULL)	{
		printf("File couldn't be opened'") ;	
		} 
		char line[1000] , x[1000];	
		char *p1;
		double eMax = 0 , ad = 1 , da = 1 ,lineNum = 0 , index = 0 ;
		int i = 0 , k , j , wtTime = 0 , started = 0   , q = 1 , totalExTime = 0 ,s ,  a1 = 0 , qa = 0;	
			while( fgets(line ,sizeof(line), fp ) != NULL ) {
				for(p1 = strtok(line," "); p1 != NULL ; p1 = strtok(NULL, " ")) {
				if(i == 0) {
				strcpy(x ,p1) ;			
				}
				if(i == 1) {
				k = atoi(p1) ;
				totalExTime += k ;
				if(k > eMax) {
					eMax = k ;
				}
				}
				if(i == 2) {
				s = atoi(p1) ;
				}
				i++ ;							
				}				
				i = 0 ;	
				addNodeToHeap(&ptr1 , x , k , s , 0 , k , k , index);	
			lineNum++ ;	
			}
		fclose(fp);
			index = 0 ; 
			while(q <= 10) {
			printf("q equals=%d\nprName   wtTime\n--------------\n" , q) ;
		double a ;	
				for( j = 0 ; j < totalExTime  ; j++) {	
				if(ptr1 != NULL) {
				a = find_minimum(&ptr1 , j , eMax , q );	// this function finds minimum element at the current moment and works on it 
				}
				if(q >= a && a - 1 >= 0)  {
					j+=  a - 1;
				}
			}
		float awt = (totalwtTime/lineNum) ; 			
		printf ("AWT = %0.f/%0.f = %0.2f\n\n" , totalwtTime , lineNum ,awt ) ;	
		FILE *fp = fopen("input.txt" , "r") ;
		ptr2 = NULL ;
		totalExTime = 0 ;	
			while( fgets(line ,sizeof(line), fp ) != NULL ) {				
				for(p1 = strtok(line," "); p1 != NULL ; p1 = strtok(NULL, " ")) {
				if(i == 0) {
				strcpy(x ,p1) ;			
				}
				if(i == 1) {
				k = atoi(p1) ;
				totalExTime += k ;
				if(k > eMax) {
					eMax = k ;
				}
				}
				if(i == 2) {
				s = atoi(p1) ;
				}
				i++ ;							
				}				
				i = 0 ;	
				addNodeToHeap(&ptr2 , x , k , s , 0 , k , k , index); 	
			}			
			ptr1 = ptr2 ;
			totalwtTime = 0 ;
			fclose(fp); 
			q++ ;
			}

		
	}
