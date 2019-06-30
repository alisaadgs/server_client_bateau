#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<time.h>

#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"
#define KBLU  "\x1B[34m"


int choix;

char vent[2];

char *nom_bateau;
int vitesse_bateau;
char direction_bateau;

int st;

char *mes;

void init_vent() //initialiser les parametres du vent
{
	vent[0]='1';
	vent[1]='N';
}


int main(int argc, char *argv[] )
{

init_vent();

printf("\n la force du vent %c et sa direction %c",vent[0],vent[1]);

int fin=0;
int portserveur;
int s;
struct sockaddr_in serveur;
char *adrserveur= (char *)malloc(32);

mes= (char *)malloc(256);
nom_bateau = (char *)malloc(256);

int force_vent;

float vitesse_final;

FILE *fptr;


// lire les parametres du bateu qui sont donnee par line de commande
if( argc ==6 ) {
	
      printf("le nom est %s\n", argv[1]);
      printf("la vitesse est %s\n", argv[2]);
      printf("la direction est %s\n", argv[3]);
      printf("X est %s\n", argv[4]);
      printf("Y  est %s\n", argv[5]);
      nom_bateau = argv[1];
      vitesse_bateau = atoi(argv[2]);
      direction_bateau = argv[3][0];
      
      force_vent = vent[0]-'0';
      printf("\n force du vent = %d",force_vent); 
      
      
      //calcule de la vitesse final initial
      if(direction_bateau==vent[1])
      {
		vitesse_final = vitesse_bateau * ( 1 + (force_vent/10) );
		printf("\nla vitesse final est %f",vitesse_final);
      }
      else
      {
      	vitesse_final = vitesse_bateau * ( 1/(force_vent));
      	printf("\nla vitesse final est %f",vitesse_final);
      }
      
      
   	 // enregistrer les paramètres dans un tableau
      char *info[]={argv[1],argv[2],argv[3],argv[4],argv[5]};    
  	//printf("\n info :%s , %s,%s",info[0],info[1],info[2]);
  	
  	
	// ouvrir le fichier des bateaux
	fptr = fopen("bateau.txt","a");  // a disigne append 

	if(fptr == NULL)
	{
		printf("Error!");   
		exit(1);             
	}

	fprintf(fptr,"%s,%s,%s,%s,%s\n",info[0],info[1],info[2],info[3],info[4]);  //ajouter les parametres des bateau au fichier
	fclose(fptr);
	
  }
   
   
   
   
   // on lit du fichier qu'il s'appelle "carte.txt" les coordonne du rocher
   
  	 FILE *file ;
  	 int counter_rocher=0;
  	 int coo_rocher[10][2];
  	 char *rocher[128];
  	 file= fopen ( "carte.txt", "r" );
	 char line[ 128 ]; /* lignes lues  */
	 char *r;

	// on envoi la taille de la carte 

	if ( file != NULL )
	{			    
		while ( fgets ( line, sizeof line, file ) != NULL ) /* lire une ligne */
		{	
			//les coordonee des rochers
			rocher[counter_rocher]=line;
			r = strtok(line," ");
			coo_rocher[counter_rocher][0] = atoi(r);   // x du rocher
			r = strtok(NULL," ");
			coo_rocher[counter_rocher][1] = atoi(r);	//y du rocher
			
			counter_rocher++;	
				
		}
		fclose ( file );
	}
	else
	{
	perror ( "carte.txt" );
	}
	
	
	// on lit les parametere des bateaux du fichier "bateau.txt" et les enrigistre dans un tableau pour chaque parametre
	
	int vitesse_b[10];
	char *direction_b[10];
	char *nom_b[10];
	int coo_bateau[10][2];
	char *b[80];
	int counter_bateau=0;
	fptr = fopen("bateau.txt","r");
	
	printf("\n\n*********** LES DONNEE DES BATEAUX ****************\n");
	
	if ( fptr != NULL )
	{			    
		while ( fgets ( line, sizeof line, file ) != NULL )
		{	
			//nom du bateau
			b[0] = strtok(line,",");
			nom_b[counter_bateau] = b[0];
			
			//vitesse du bateau
			b[1] = strtok(NULL,",");
			vitesse_b[counter_bateau] = atoi(b[1]);
			
			//direction du bateau
			b[2] = strtok(NULL,",");
			direction_b[counter_bateau] = b[2];
			
			//les coordonees du bateau
			b[3] = strtok(NULL,",");
			coo_bateau[counter_bateau][0] = atoi(b[3]);   //x du bateau
			b[4] = strtok(NULL,",");
			coo_bateau[counter_bateau][1] = atoi(b[4]);	// y du bateau


		
			// afficher les paramètres du bateau enregistrés dans le fichier des bateaux
			printf("\n %d) bateau:",counter_bateau+1);
			printf("\n------------");
			printf("\n nom du bateau %d est :%s",counter_bateau+1,nom_b[counter_bateau]);
			printf("\n vitesse du bateau %d est :%d",counter_bateau+1,vitesse_b[counter_bateau]);
			printf("\n direction du bateau %d est :%s",counter_bateau+1,direction_b[counter_bateau]);
			printf("\n x%d=%d",counter_bateau,coo_bateau[counter_bateau][0]);
			printf("\ty%d=%d \n",counter_bateau,coo_bateau[counter_bateau][1]);
			printf("\n \t *******");


			counter_bateau++;	
				
		}
		fclose ( fptr );
	}
	else
	{
	perror ( "bateau.txt" ); 
	}
	
	
	
	// affiche des cordonnees des rochers
	int i=0;
	for(i=0;i<counter_rocher;i++)
	{
		printf("\nx%d=%d",i,coo_rocher[i][0]);
		printf("\ny%d=%d",i,coo_rocher[i][1]);
	}
	
	


printf("\nProgramme Client TCP\n");


do
{
	adrserveur="127.0.0.1";
	if(inet_aton(adrserveur,&serveur.sin_addr)!=0)
		fin=2;
	else
	printf("Adresse invalide,recommencer ou fin pour terminet :");

}while(fin==0);

if((fin==2))
{


	portserveur=4443;
	printf("Appel Fonction Socket\n");
	s=socket(AF_INET,SOCK_STREAM,6);
	if(s==-1)
	{
		printf("Retour socket invalide impossible de continuer");
		return (-1);
	}
	printf("Demande de connexion\n");
	serveur.sin_family=AF_INET;
	serveur.sin_port=htons(portserveur);
	st=connect(s,(const struct sockaddr *)&serveur,sizeof(serveur));
	if(st!=0)
	{
		printf("Connexion Refuse");
		return (-1);
	}
	printf("Connxion etablie attente\n");


	
	// fonction pour thread bateau
	
	void gestionnaire_bateau() {
	
		while(1)
		{

			printf("\n\n***************************************************************");
			printf("\n***********************  MENU  ********************************");
			printf("\n***************************************************************");
	
			printf("\n 1) Modification de la vitesse du bateaux");
			printf("\n 2) Modification de la direction du bateaux");
			printf("\n 3) visualisation de l ocean");
			printf("\n 4) QUIT");
	
			printf("\n donner votre choix:");
			scanf("%d",&choix);
	
			switch(choix)
			{
				case 1: 
					{				
						int a;
						printf("\n changer la vitesse du bateaux :");
						scanf("%d",&vitesse_bateau);
						for(a = 0; a < counter_bateau ; a++)
						{
							if( strcmp(nom_b[a],nom_bateau) ==0)
							{
								vitesse_b[a] = vitesse_bateau;
								break;
							}
						}
						printf("\n la nouvelle vitesse est: %d", vitesse_bateau);
						
					} break;
			
				
				case 2: 
					{
						int a;
						printf("\n changer la direction du bateau:");
						scanf(" %c",&direction_bateau);
						for(a = 0; a < counter_bateau ; a++)
						{
							if( strcmp(nom_b[a],nom_bateau) ==0)
							{
								direction_b[a][0] = direction_bateau;
								break;
							}
						}
						printf("\n la nouvelle direction est: %c",direction_b[a][0]);
						
					} break;
					
					
				case 3: 
				{
					int dim_ocean=15;
					int j;
					int r;
					int b;
					int rocher_trouver=0;  //variable pour tester s'il existe un rocher ou pas
					int bateau_trouver=0;
					char ocean[dim_ocean][dim_ocean];
					force_vent = vent[0]-'0';
					
					for(i=1;i<=dim_ocean;i++)
					{
						for(j=1;j<=dim_ocean;j++)
						{
							// on cherche si il y a des rochers
							rocher_trouver = 0;
							for(r=0; r<counter_rocher;r++)
							{
								if( (i==coo_rocher[r][0]) && (j == coo_rocher[r][1]))
								{
									rocher_trouver = 1;
								}
								
							}
							
							if(rocher_trouver == 1)
							{ 
								printf("%sR ", KRED);
								ocean[i][j]='R';
							} 
							
							// on cherche si il y a des bateaux
							bateau_trouver = 0;
							for(b=0; b<counter_bateau;b++)
							{
								if( (i==coo_bateau[b][0]) && (j == coo_bateau[b][1]))
								{
									bateau_trouver = 1;
								}
								
							}
							if(bateau_trouver == 0 && rocher_trouver == 0)
							{
								printf("%sO ",KBLU);
								ocean[i][j]='O';
							}
							else if(bateau_trouver == 1)
							{ 
								printf("%sB ", KNRM);
								ocean[i][j]='B'; 
							} 
						}
						printf("%s\n",KNRM);
					}
					int c;
					int t;
					float sleep1 = 0; // temps pour fair sleep selon les vitesse
					int sleep_micro;
					
					r = 0 ; //conteur du rochers
					printf("\n ");
					for(c=0;c<10;c++)
					{
						//sleep(1);
						
						system("clear");
						for(t=0;t<counter_bateau;t++)
						{
							
							// Calcule du vitesse final
							if(direction_b[t][0]==vent[1])
							{
								vitesse_final = vitesse_b[t] * ( 1 + ((force_vent/10)) );
								//printf("\nla vitesse final est %d",vitesse_final);
							}
							else
							{
								vitesse_final = vitesse_b[t] * ( 1/force_vent );
								//printf("\nla vitesse final est %d",vitesse_final);
							}
							
							// time to sleep : si la vitesse est 20 alors chaque 1 seconde le bateau marche
							sleep1 = 20/vitesse_final;
							sleep_micro = sleep1 * 1000000;
							 
							// *************************** comment boujer le bateau dans l'ocean *****************************
							// la direction E
							
							if( strcmp(direction_b[t],"E") ==0 )
							{
								
								coo_bateau[t][1]++;
								if(coo_bateau[t][1] == dim_ocean+1)
								{
									coo_bateau[t][1] = 1;
								}
								if(r < counter_rocher)
								{
									if( (coo_bateau[t][1] == coo_rocher[r][1]) && (coo_bateau[t][0] == coo_rocher[r][0]) )
									{
										coo_bateau[t][1]++;
										sleep_micro = sleep_micro*2;
										r++; // on ajoute le counter de rocher
										if( (coo_bateau[t][1] == coo_bateau[t+1][1]) && (coo_bateau[t][0] == coo_bateau[t+1][0]) )
										{
											coo_bateau[t][1]++;
											sleep_micro = sleep_micro*2;
										}	
									}
								}

								if( (coo_bateau[t][1] == coo_bateau[t+1][1]) && (coo_bateau[t][0] == coo_bateau[t+1][0]) )
								{
									coo_bateau[t][1]++;
									sleep_micro = sleep_micro*2;
									if(r < counter_rocher)
									{
										if( (coo_bateau[t][1] == coo_rocher[r][1]) && (coo_bateau[t][0] == coo_rocher[r][0]) )
										{
											coo_bateau[t][1]++;
											sleep_micro = sleep_micro*2;
											r++; // on ajoute le counter de rocher
										}
									}
								}	
							} // fin du direction E
							
							
							// la direction W
							if(strcmp(direction_b[t],"W") ==0)
							{
								coo_bateau[t][1]--;
								if(coo_bateau[t][1] == 0)
								{
									coo_bateau[t][1] = dim_ocean;
								}
								if(r < counter_rocher)
								{
									if( (coo_bateau[t][1] == coo_rocher[r][1]) && (coo_bateau[t][0] == coo_rocher[r][0]) )
									{
										coo_bateau[t][1]--;
										sleep_micro = sleep_micro*2;
										r++; // on ajoute le counter de rocher
										if( (coo_bateau[t][1] == coo_bateau[t-1][1]) && (coo_bateau[t][0] == coo_bateau[t-1][0]) )
										{
											coo_bateau[t][1]--;
											sleep_micro = sleep_micro*2;
										}	
									}
								}

								if( (coo_bateau[t][1] == coo_bateau[t-1][1]) && (coo_bateau[t][0] == coo_bateau[t-1][0]) )
								{
									coo_bateau[t][1]--;
									sleep_micro = sleep_micro*2;
									if(r < counter_rocher)
									{
										if( (coo_bateau[t][1] == coo_rocher[r][1]) && (coo_bateau[t][0] == coo_rocher[r][0]) )
										{
											coo_bateau[t][1]--;
											sleep_micro = sleep_micro*2;
											r++; // on ajoute le counter de rocher
										}
									}
								}	
							} // fin du direction W
							
							
							
							// la direction S
							
							if( strcmp(direction_b[t],"S") ==0 )
							{
								
								coo_bateau[t][0]++;
								if(coo_bateau[t][0] == dim_ocean+1)
								{
									coo_bateau[t][0] = 1;
								}
								if(r < counter_rocher)
								{
									if( (coo_bateau[t][1] == coo_rocher[r][1]) && (coo_bateau[t][0] == coo_rocher[r][0]) )
									{
										coo_bateau[t][0]++;
										sleep_micro = sleep_micro*2;
										r++; // on ajoute le counter de rocher
										if( (coo_bateau[t][1] == coo_bateau[t+1][1]) && (coo_bateau[t][0] == coo_bateau[t+1][0]) )
										{
											coo_bateau[t][0]++;
											sleep_micro = sleep_micro*2;
										}	
									}
								}

								if( (coo_bateau[t][1] == coo_bateau[t+1][1]) && (coo_bateau[t][0] == coo_bateau[t+1][0]) )
								{
									coo_bateau[t][0]++;
									sleep_micro = sleep_micro*2;
									if(r < counter_rocher)
									{
										if( (coo_bateau[t][1] == coo_rocher[r][1]) && (coo_bateau[t][0] == coo_rocher[r][0]) )
										{
											coo_bateau[t][0]++;
											sleep_micro = sleep_micro*2;
											r++; // on ajoute le counter de rocher
										}
									}
								}	
							} // fin du direction S
							
							
							
							// la direction N
							if(strcmp(direction_b[t],"N") ==0)
							{
								coo_bateau[t][0]--;
								if(coo_bateau[t][0] == 0)
								{
									coo_bateau[t][0] = dim_ocean;
								}
								if(r < counter_rocher)
								{
									if( (coo_bateau[t][1] == coo_rocher[r][1]) && (coo_bateau[t][0] == coo_rocher[r][0]) )
									{
										coo_bateau[t][0]--;
										sleep_micro = sleep_micro*2;
										r++; // on ajoute le counter de rocher
										if( (coo_bateau[t][1] == coo_bateau[t-1][1]) && (coo_bateau[t][0] == coo_bateau[t-1][0]) )
										{
											coo_bateau[t][0]--;
											sleep_micro = sleep_micro*2;
										}	
									}
								}

								if( (coo_bateau[t][1] == coo_bateau[t-1][1]) && (coo_bateau[t][0] == coo_bateau[t-1][0]) )
								{
									coo_bateau[t][0]--;
									sleep_micro = sleep_micro*2;
									if(r < counter_rocher)
									{
										if( (coo_bateau[t][1] == coo_rocher[r][1]) && (coo_bateau[t][0] == coo_rocher[r][0]) )
										{
											coo_bateau[t][0]--;
											sleep_micro = sleep_micro*2;
											r++; // on ajoute le counter de rocher
										}
									}
								}	
							} // fin du direction N
							
							
							
							
						}
						
						printf("\n\n %d\n",c);
						for(i=1;i<=dim_ocean;i++)
						{
							for(j=1;j<=dim_ocean;j++)
							{
								// on cherche s'il y a des rochers
								rocher_trouver = 0;
								for(r=0; r<counter_rocher;r++)
								{
									if( (i==coo_rocher[r][0]) && (j == coo_rocher[r][1]))
									{
										rocher_trouver = 1;
									}
								
								}
							
								if(rocher_trouver == 1)
								{ 
									printf("%sR ", KRED);
									ocean[i][j]='R';
								} 
							
								// on cherche s'il y a des bateaux
								bateau_trouver = 0;
								for(b=0; b<counter_bateau;b++)
								{
									if( (i==coo_bateau[b][0]) && (j == coo_bateau[b][1]))
									{
										bateau_trouver = 1;
									}
								
								}
								if(bateau_trouver == 0 && rocher_trouver == 0)
								{
									printf("%sO ",KBLU);
									ocean[i][j]='O';
								}
								else if(bateau_trouver == 1)
								{ 
									printf("%sB ", KNRM);
									ocean[i][j]='B'; 
								} 
							}
							printf("%s\n",KNRM);
						}
						
						usleep(sleep_micro);
						system("clear");
						
					}
					
				} break;
				
				case 4:
				{
					
				} break;
					
					
		
			}
	
	
	
		     }
		
	}
	
	
	
	
	
	
	// fonction pour thread recevoir vent
	void recevoir_vent(){
		
		while(1){
	  	
	   	
	  	printf("\nAttente si il ya un changement du vent \n\n");	   

		//reception des coordonees du bateau
		st=recv(s,mes,sizeof(mes),0);

		if(st>0)
		{
			mes[st]='\0';
			vent[0] = mes[0];
			vent[1] = mes[1];

		}
		printf("vent : vitesse %c et la direction:%c\n ",vent[0],vent[1]);
	    }
	
	}
	
	
	
	
	
	
	
	
	// threads
	

	

	pthread_t thread_bateau;
        if (pthread_create(&thread_bateau, NULL, (void *)gestionnaire_bateau, NULL)) {
            perror("Pthread erreur!\n");
            exit(EXIT_FAILURE);
        }
        
      //pthread_join(thread_bateau, NULL); 
        
        pthread_t thread_vent;
        if (pthread_create(&thread_vent, NULL, (void * )recevoir_vent, NULL)) {
            perror("Pthread erreur!\n");
            exit(EXIT_FAILURE);
        }

	 
	 pthread_join(thread_vent, NULL); // attendre la fin du thread


   }


return 0;
}


