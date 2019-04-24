#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

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

/* Lit le fichier du graphe web et rempli les structures */
TabListe* read_file(char* path, int* size){
    FILE* fd = NULL;
    TabListe* tabN;
    long double var=0.0, vartmp=0.0;
    long double moy = 0;

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
    moy = (double)tmp/(*size);
    tabN= calloc(*size, sizeof(TabListe));

    for(int i=0;i<*size;i++){
        fscanf(fd,"%d ",&iTmp);
        fscanf(fd,"%d ",&degTmp);
        vartmp = (double)degTmp/(*size)-moy;
        vartmp=vartmp*vartmp;
        var+=vartmp;
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
   
     printf("Var : %LF\n",var/(*size));
    fclose(fd);
    return tabN;
}

//affiche les pertinances de chaque pages
void print_pi(double pi[], int n){
    double somme=0.0;
    for(int i= 0; i<n; i++){
        if(i<24)
            printf("page %d : %lf\n ",i+1, pi[i]);
        if(isinf(pi[i]) || isnan(pi[i]))
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

/* Calcul du pagerank par l'algorithme basique */
void power(double* pi, TabListe* tab, int n){
	double somme;
    double* piTmp = malloc(n*sizeof(double));
    init_pi(piTmp,n,1.0/(double)n);
    Liste tmp;
    double convergence=1.0;
    double diff_abs, c1 = (double)(1-alpha)/n, c2;
    int itt = 0;
	while(convergence>epsilon){
		convergence = 0.0;
		
        //initialisation du vecteur f et de pi et piTmp
        somme=0;
        for(int i=0;i<n; i++){
            pi[i]=piTmp[i];
            piTmp[i]=0;
            
            if(tab[i].deg==0)
                somme+=pi[i];
        }
        c2 = (somme*alpha)/(double)n;
        itt++;
	    for(int i=0; i<n;i++){
	        tmp=tab[i].l;
   	        while(tmp!=NULL){
	            piTmp[i]+=(pi[tmp->i-1])*(tmp->val);
	            tmp=tmp->next;
	        }
            piTmp[i]*=alpha;
            piTmp[i]+=c1 + c2;
            diff_abs = pi[i] - piTmp[i];
            convergence +=  diff_abs >= 0.0 ? diff_abs : 0.0-diff_abs;
	    }
	}
	free(piTmp);
    printf("itt = %d\n", itt);
   
}

/* Calcul du pagerank par l'algorithme de Gauss Seidel */
void power_Seidel(double* pi, TabListe* tab, int n){
    double somme;
    double* piTmp = malloc(n*sizeof(double));
    init_pi(piTmp,n,1.0/(double)n);
    Liste tmp;
    double convergence=1.0;
    double diff_abs, c1 = (1-alpha)/(double)n, c2;
    double no, Gi;
    int itt = 0;
    
       
    while(convergence>epsilon){
        convergence = 0.0;
        itt++;
        no=0;
        //initialisation du vecteur f, pi et piTmp
        somme=0;
        for(int i=0;i<n; i++){
            pi[i]=piTmp[i];
            piTmp[i]=0.0;
            if(tab[i].deg==0)
                somme+=pi[i];
                
        }
        c2 = (somme*alpha)/(double)n;
        for(int i=0; i<n; i++){
            tmp=tab[i].l;
            /* On commence par la somme S(de j=i+1 à n)=x^k*G[j,i] car tab[i].l contient la colonne
            par ordre de sommets décroissant sur j de G[j,i].
            tmp->i-1 représente l'indice j de la somme */
            while(tmp!=NULL && (tmp->i-1) > i){
                piTmp[i]+=(pi[tmp->i-1])*(tmp->val);
                tmp=tmp->next;
            }
            if(tmp != NULL && tmp->i == (i+1)){
                Gi = tmp->val;
                tmp = tmp->next;
            }
            else
                Gi=0.0;
            while(tmp!=NULL){
                piTmp[i]+=(piTmp[tmp->i-1])*(tmp->val);
                tmp=tmp->next;
            }
            piTmp[i]*=alpha;
            piTmp[i]+=c1 + c2;
            piTmp[i]=piTmp[i]/(1.0-(1.0-alpha)*Gi/n-c2);
            
            no+=piTmp[i];
        }
 
        //renormalisation de pi et calcul de la convergence
        for (int k=0;k<n;k++){
            piTmp[k]=piTmp[k]/no;
            diff_abs = pi[k] - piTmp[k];
            convergence +=  diff_abs >= 0.0 ? diff_abs : 0.0-diff_abs;
        }
    }

    printf("itt = %d\n", itt);
    free(piTmp);
}


int main(int argc, char** argv){    //argv[1] = chemin vers le graphe à analyser
    float temps;                    //argv[2] = "seidel" --> algo power seidel sinon algo basique
    clock_t t1, t2;
    int n =0;
    TabListe* tabListe;
    tabListe=read_file(argv[1], &n);
    double* pi= malloc(n*sizeof(double));
    printf("Lecture du graphe effectuee! \n");
	printf("\nEffectue power\n");
    t1 = clock();
    power(pi, tabListe, n);
    t2 = clock();
    //print_pi(pi, n);
    printf("algo power effectue! \n");
    temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    printf("temps = %f\n", temps);
    printf("\nEffectue Gauss-Seidel\n");
    t1 = clock();
	power_Seidel(pi, tabListe, n);
    t2 = clock();
    //print_pi(pi, n);
    printf("algo Gauss-Seidel effectue! \n");
    temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    printf("temps = %f\n", temps);
    free(pi);
    free_tabListe(tabListe, n);
    return 0;
}
