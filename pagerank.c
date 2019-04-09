#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define epsilon pow(10,-12)
#define alpha 0.85

// déclaration de la structure Liste qui est une liste chainée simple
struct element{
    unsigned int i; // le numero de la ligne sur lequel se trouve la valeur de l'Element
    unsigned int j; // le numero de la colonne sur lequel se trouve la valeur de l'Element (à voir si ce champs est nécessaire)
    double val;      // la valeur flottante de la matrice contenue dans cet Element
    struct element* next; // l'adresse de l'Element suivant
};
typedef struct element Element;
typedef Element* Liste;

struct tabListe{
    unsigned int deg; //degré sortant
    Liste l;
};
typedef struct tabListe TabListe;

void free_Liste(Liste l){
    if(l!=NULL){
        free_Liste(l->next);
        free(l);
    }
}

void free_tabListe(TabListe* tabListe, int n){
    for(int i=0; i<n; i++)
        free_Liste(tabListe[i].l);
}

// Création et initialisation d'un nouvel Element avec les valeurs passées en paramètre de Liste et renvoie d'un pointeur sur son adresse
Element* new_Elt(int i, int j, double m){
    Element* elt= malloc(sizeof(Element));
    elt->i=i;
    elt->j=j;
    elt->val=m;
    elt->next=NULL;

    return elt;
}

// ajoute l'Element et au début de la liste l
Liste add_elt(Liste l, Element* elt){
    elt->next=l;
    return elt;
}

TabListe* read_file(char* path, int* size){
    FILE* fd = NULL;
    TabListe* tabN;

    fd = fopen(path, "r");

	if (fd == NULL){
		fprintf(stderr, "null file descriptor\n");
		exit(EXIT_FAILURE);
	}
	
    int iTmp,jTmp,tmp,degTmp;
    double lectFloat;
    Element* elt;
    // on lit n et n
    fscanf(fd,"%d\n%d\n",size,&tmp);
    tabN= calloc(*size, sizeof(TabListe));

    for(int i=0;i<*size;i++){
        fscanf(fd,"%d ",&iTmp);
        fscanf(fd,"%d ",&degTmp);
        tabN[i].deg=degTmp;
        for(int y=0;y<degTmp;y++){
            if(y==degTmp-1)
                fscanf(fd,"%d %lf\n",&jTmp,&lectFloat);
            else
                fscanf(fd,"%d %lf ",&jTmp,&lectFloat);
            if((iTmp > *size) || (jTmp > *size)){
            	fprintf(stderr, "Erreur sur le nombre de sommets\n");
            	printf("n : %d 	i :%d 	j : %d\n", *size, iTmp, jTmp);
            	free_tabListe(tabN, *size);
            	exit(EXIT_FAILURE);
            }
            elt=new_Elt(iTmp,jTmp,lectFloat);
            tabN[(elt->j)-1].l = add_elt(tabN[(elt->j)-1].l,elt);
        }
    }
    //printTab(tabN,*size);
    fclose(fd);
    return tabN;
}

//affiche les pertinances de chaque pages
void print_pi(double pi[], int n){
    double somme=0.0;
    for(int i= 0; i<n; i++){
        if(i<20)
            printf("page %d : %lf\n ",i+1, pi[i]);
        somme+=pi[i];
    }
    printf("Somme: %lf\n", somme);
}

//initialise le vecteur pi avec la valeur val
void init_pi(double* pi, int size, double val){
	for(int i=0; i<size; i++)
		pi[i] = val;
}

// affiche la totalité des valeurs contenue dans le tableau de Liste représentant la matrice creuse
void printTab(TabListe* tab, int size){
    Liste tmp;
    printf("size: %d\n", size);
    for(int i=0;i<size;i++){
        tmp=tab[i].l;
        while(tmp!=NULL){
            printf("%d %d %lf\n", tmp->i,tmp->j,tmp->val);
            tmp=tmp->next;
        }
    }
}

void power(double* pi, TabListe* tab, int n){
	double somme;
    double* piTmp = malloc(n*sizeof(double));
    Liste tmp;
    double convergence=1.0;
    int itt = 0;

	while(convergence>epsilon){
		init_pi(piTmp,n,0.0);
		// for(int i=0; i<n; i++){
		// 	piTmp[i] = 0.0;
		// }
		convergence = 0.0;
		itt++;
	    for(int i=0; i<n; i++){
	        tmp=tab[i].l;
	        while(tmp!=NULL){
	            piTmp[i]+=(pi[tmp->i-1])*(tmp->val);
	            //printf("Pour j=%d et i=%d :pi%lf %lf x %lf \n",tmp->j,tmp->i,piTmp[i],pi[tmp->i-1],tmp->val );
	            tmp=tmp->next;
	        }
	    }
	    //initialisation du vecteur f
	    somme=0;
	    for(int i=0;i<n; i++){
	        if(tab[i].deg==0)
	            somme+=pi[i];
	    }

	    double c1 = (1-alpha)/n, c2=(somme*alpha)/n;
	    //printf(" valeur de sigma: %lf ",c2);
	    for(int i=0; i<n; i++){
	        piTmp[i]*=alpha;
	        piTmp[i]+=c1;
	        piTmp[i]+=c2;
	        double diff_abs = pi[i] - piTmp[i];
	        convergence +=  diff_abs >= 0.0 ? diff_abs : 0.0-diff_abs;//diff_absolue(pi[i],piTmp[i]);
	        pi[i]=piTmp[i];
	    }
	}
	free(piTmp);
}

void power_Seidel(double* pi, TabListe* tab, int n){
    double somme;
    double* piTmp = malloc(n*sizeof(double));
    Liste tmp;
    double convergence=1.0;
    int itt = 0;

    while(convergence>epsilon){
        init_pi(piTmp,n,0.0);
        // for(int i=0; i<n; i++){
        //  piTmp[i] = 0.0;
        // }
        convergence = 0.0;
        itt++;
        for(int i=0; i<n; i++){
            for(int j=0;j<i-1;j++){
                tmp=tab[j].l;
                while(tmp!=NULL){
                    piTmp[i]+=(piTmp[j])*(tmp->val);
                    tmp=tmp->next;
                }
            }
            for(int j=i+1;j<n;j++){
                tmp=tab[j].l;
                while(tmp!=NULL){
                    piTmp[i]+=(pi[j])*(tmp->val);
                    tmp=tmp->next;
                }
            }
            tmp=tab[i].l;
            while(tmp!=NULL && tmp->i!=i) tmp=tmp->next;
            if(tmp!=NULL)piTmp[i]=piTmp[i]/(1-tmp->val);
            
        }
        //initialisation du vecteur f
        somme=0;
        for(int i=0;i<n; i++){
            if(tab[i].deg==0)
                somme+=pi[i];
        }

        double c1 = (1-alpha)/n, c2=(somme*alpha)/n;
        //printf(" valeur de sigma: %lf ",c2);
        for(int i=0; i<n; i++){
            piTmp[i]*=alpha;
            piTmp[i]+=c1;
            piTmp[i]+=c2;
            double diff_abs = pi[i] - piTmp[i];
            convergence +=  diff_abs >= 0.0 ? diff_abs : 0.0-diff_abs;//diff_absolue(pi[i],piTmp[i]);
            pi[i]=piTmp[i];
        }
    }
    free(piTmp);
}


int main(){
    float temps;
    clock_t t1, t2;
    t1 = clock();
	int n =0;
    TabListe* tabListe;
    tabListe=read_file("graph/wikipedia-20051105/wikipedia-20051105V2.txt", &n);
    double* pi= malloc(n*sizeof(double));
    init_pi(pi, n, 1.0/(double)n);
    //printTab(tabListe, n);
    printf("Lecture du graphe effectuee! \n");
    //convergence_produit(pi,tabListe,n);
    power_Seidel(pi, tabListe, n);
    printf("algo power effectuee! \n");
    print_pi(pi, n);
    free(pi);
    free_tabListe(tabListe, n);
    t2 = clock();
    temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    printf("temps = %f\n", temps);
    return 0;
}