//AVLtree.cpp : 定义控制台应用程序的入口点。


#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct avlt {
	int value;
	struct avlt *lch;
	struct avlt *rch;
	int lh;
	int rh;
}btnode;
int getheight(btnode* T);
int dist(int a, int b);
btnode* insert(btnode* ot,int key);
btnode* findpre(btnode* T, btnode * p);
btnode* rotate(btnode *T);
btnode* isbalance(btnode *T);
btnode* delete_node(btnode* T, int value);
btnode* findmax(btnode* T);
btnode* findmin(btnode* T);
void updateh(btnode*T);
void FOT(btnode* T);
int main()
{
	int n;
	printf("input the amount of numbers:");
	scanf_s("%d", &n);
	printf("input %d integers:",n);
	int *a=(int*)malloc(n*sizeof(int));
	int i,num;
	btnode* T=NULL;
	btnode* p;
	for (i = 0;i < n;i++) {
		scanf_s("%d", &a[i]);	
	}
	for (i = 0;i < n;i++) {
		T = insert(T, a[i]);
		p = isbalance(T);
		if (p != NULL) {
			printf("rotation point:%d\n", p->value);
			T = rotate(T);
			updateh(T);
			FOT(T);
		}
		else {
			printf("no rotation this insertion!\n");
			FOT(T);
		}
	}
	printf("input the node you want to delete:");
	scanf_s("%d",&num);
	T=delete_node(T, num);
	updateh(T);
	FOT(T);
	system("pause");
    return 0;
}
btnode* insert(btnode* T, int key) {
	int situation;
	if (T == NULL) {
		T = (btnode*)malloc(sizeof(btnode));
		T->lch = NULL;
		T->rch = NULL;
		T->value = key;
		T->lh = 0;
		T->rh = 0;
	}
	else {
		if (T->value > key) {
			T->lch = insert(T->lch, key);
		}
		else {
			T->rch = insert(T->rch, key);
		}
	}
	updateh(T);//已经重新加载过左右子树的高度
	return T;
}
int getheight(btnode *T) {
	if (T == NULL) {
		return -1;
	}
	else {
		if (getheight(T->lch) > getheight(T->rch))
			return getheight(T->lch) + 1;
		else
			return getheight(T->rch) + 1;
	}
}
void updateh(btnode*T) {
	if (T != NULL) {
		updateh(T->lch);
		updateh(T->rch);
		T->lh = getheight(T->lch);
		T->rh = getheight(T->rch);
	}
}
void FOT(btnode* T) {//先序遍历
	if (T != NULL) {		
		printf("value is %d,lh=%d,rh=%d\n", T->value, T->lh, T->rh);
		FOT(T->lch);
		FOT(T->rch);
	}
}
btnode* findpre(btnode* T,btnode * p) {
	btnode* position;
	if (p == T) {
		return NULL;
	}
	else {
		position = T;
		while (position->lch!=p&&position->rch!=p)
		{
			if (position->value > p->value) {
				position = position->lch;
			}
			else {
				position = position->rch;
			}
		}
		return position;
	}
}
int dist(int a, int b) {
	if (a > b)
		return a - b;
	else
		return b - a;
}
btnode* isbalance(btnode *T) {
	btnode* p=T,*pl, *pr;
	if (T == NULL) {
		return NULL;//即没有不平衡点
	}
	else {//T非空
		pl = isbalance(T->lch);
		pr = isbalance(T->rch);//考察他的两个子树的平衡情况，再做决定 
		if (dist(T->lh,T->rh)<=1) {//T在根处平衡
			if (pl == NULL&&pr == NULL) {//子树全部平衡
				return NULL;
			}
			else {
				if (pl != NULL) {
					p = pl;
				}
				else {
					if (pr != NULL) {
						p = pr;
					}
				}
				return p;
			}
		}
		else if(dist(T->lh, T->rh) >= 2){
			if (pl == NULL&&pr == NULL) {//子树全部平衡
				return p;
			}
			else {
				if (pl != NULL) {
					p = pl;
				}
				else {
					if (pr != NULL) {
						p = pr;
					}
				}
				
				return p;
			}
		}
			

	}
}//判断当前树在任意节点处是否是平衡，平衡，返回NULL，否则返回最深的需要旋转的地方——只可能有一处
btnode* rotate(btnode *T) {
	btnode* position = isbalance(T);
	btnode* pf = findpre(T, position);
	btnode* ps, *pss;
	if (pf != NULL) {//存在父节点
		if (position->lh>position->rh&&position->lch->lh>position->lch->rh) {//case 1:左左单旋转
			ps = position->lch;
			pss = ps->lch;
			if (pf->lch == position) 
				pf->lch = ps;
			else
				pf->rch = ps;
			position->lch = ps->rch;
			ps->rch = position;
		}
		else if (position->lh>position->rh&&position->lch->rh>position->lch->lh) {//case 2:左右双旋转
			ps = position->lch;
			pss = ps->rch;
			if (pf->lch == position)
				pf->lch = pss;
			else
				pf->rch = pss;
			ps->rch = pss->lch;
			position->lch = pss->rch;
			pss->lch = ps;
			pss->rch = position;
		}
		else if (position->lh<position->rh&&position->rch->lh<position->rch->rh) {//case 3:右右单旋转
			ps = position->rch;
			pss = ps->rch;
			if (pf->lch == position)
				pf->lch = ps;
			else
				pf->rch = ps;
			position->rch = ps->lch;
			ps->lch = position;
		}
		else if (position->lh<position->rh&&position->rch->lh>position->rch->rh) {//case 4:右左双旋转
			ps = position->rch;
			pss = ps->lch;
			if (pf->lch == position)
				pf->lch = pss;
			else
				pf->rch = pss;
			position->rch = pss->lch;
			ps->lch = pss->rch;
			pss->lch = position;
			pss->rch = ps;
		}
	}
	else {//在这种情况下，position就是根
		if (position == T) {

			if (position->lh > position->rh&&position->lch->lh > position->lch->rh) {//case 1:左左单旋转
				ps = position->lch;
				pss = ps->lch;
				T = ps;
				position->lch = ps->rch;
				ps->rch = position;
			}
			else if (position->lh > position->rh&&position->lch->rh > position->lch->lh) {//case 2:左右双旋转
				ps = position->lch;
				pss = ps->rch;
				T = pss;
				ps->rch = pss->lch;
				position->lch = pss->rch;
				pss->lch = ps;
				pss->rch = position;
			}
			else if (position->lh < position->rh&&position->rch->lh < position->rch->rh) {//case 3:右右单旋转
				ps = position->rch;
				pss = ps->rch;
				T = ps;
				position->rch = ps->lch;
				ps->lch = position;
			}
			else if (position->lh<position->rh&&position->rch->lh>position->rch->rh) {//case 4:右左双旋转
				ps = position->rch;
				pss = ps->lch;
				T = pss;
				position->rch = pss->lch;
				ps->lch = pss->rch;
				pss->lch = position;
				pss->rch = ps;
			}

		}
	}
	return T;
}
btnode* find(btnode* T, int num) {
	btnode* res=NULL;
	if (T == NULL) {
		return NULL;//没找到
	}
	else {
		if (num < T->value) {
			res = find(T->lch, num);
		}
		else if (num>T->value) {
			res = find(T->rch, num);
		}
		else if (num == T->value) {
			res = T;
		}
		return res;
	}
}
btnode* findmax(btnode* T) {
	btnode* res;
	if (T->rch != NULL) {
		res = findmax(T->rch);
	}
	else {
		res = T;
	}
	return res;
}
btnode* findmin(btnode* T) {
	btnode* res;
	if (T->lch != NULL) {
		res = findmin(T->lch);
	}
	else {
		res = T;
	}
	return res;
}
btnode* delete_node(btnode* T, int value) {
	//删除的实际上是比他小的最大节点
	btnode* fop,*p1,*fop1;
	btnode* p = find(T, value);//先找到value所在的位置，真正删除的并不一定是它
	if (p->lch == NULL&&p->rch == NULL) {
		fop = findpre(T, p);
		if (fop->lch == p)
			fop->lch = NULL;
		else
			fop->rch = NULL;
	}
	else if (p->lch != NULL) {
		btnode* p1 = findmax(p->lch);
		fop1 = findpre(T, p1);
		fop = findpre(T, p);
		if (fop->lch == p)
			fop->lch = p1;
		else
			fop->rch = p1;
		p1->lch = p->lch;
		p1->rch = p->rch;
		if (fop1->lch == p1)
			fop1->lch = NULL;
		else
			fop1->rch = NULL;
	}
	else {
		btnode* p1 = findmin(p->rch);
		fop = findpre(T, p);
		if (fop->lch == p)
			fop->lch = p1;
		else
			fop->rch = p1;
		fop1 = findpre(T, p1);
		p1->lch = p->lch;
		p1->rch = p->rch;
		if (fop1->lch == p1)
			fop1->lch = NULL;
		else
			fop1->rch = NULL;
	}
	return T;
}
