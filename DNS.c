#include "head.h"

// Auteur :  Aboelnaga Ahmed


//char name[1000];

// fonction d'affichage du domaine name / IP
int Namehere(unsigned char* query,int deb,int d,int dd){
    char * q = query+deb;
    int i=0,s=1;
    for(i;q[i]&&s;){
        unsigned char B1 = q[i++];
        (i-1==0)? 1 : printf(".");
        if (B1 > 191 ){
            unsigned char B2 = q[i];
            unsigned char deb2 = offset(B1,B2);
            Namehere(query,deb2,d,dd);
            s=0;
        }
        else{
            int off = B1+i;
            for (i;i<off;i++) printf("%c",q[i]);
        }
    }
    (i==0) ? printf("Root"):1;
    printf("\n");
    return i+1;
}

void aff(int a){

    switch(a){
        case 1000 : printf("\t (IN) ");break;
        case IP4: printf("\t (AA) ");break;
        case IP6 : printf("\t (AAAA)");break;
        case SOA : printf("\t (Start of Authority Area SOA)");break;
        case C_N  : printf("\t (Name for an alias)");break;
        case N_S : printf("\t (Authoritative Name Server) ");break;
        default : break;
    }
}

void affques(DNS_q* responseqw){
        printf(";; Type : %d ",ntohs(responseqw->QTYPE));aff(ntohs(responseqw->QTYPE));
        printf("   Class : %d ",ntohs(responseqw->QCLASS));aff(1000);
        printf("\n");
}

void affipv4(unsigned char* data){
    FILE* fd = fopen("ip.txt","w");
    printf(";; IP : ");
    for (int io=0;io<4;io++){
        fprintf(fd,"%d",data[io]);
        if(io!=3) {
            printf("%d.",data[io]) ;
            fprintf(fd,"%c",'.');
        }
        else {
            printf("%d",data[io]) ; 
        }   
    } 
    printf("\t\tIN\t\tAA\n\n");
}

void affipv6(unsigned char* data){
    printf(";; IP : ");
    for (int io=0;io<15;io+=2) {
        printf("%02hhx%02hhx",data[io],data[io+1]) ;
        (io!=14) ? printf(":") : 1 ;
    }
    printf("\t\tIN\t\tAAAA\n");
}


void init_header(DNS_h* header){
    header->ID = (unsigned short) htons(getpid());
    header->RD=1;
    header->QDCOUNT=htons(1);
};

void init_question(DNS_q* q,int x){
    q->QTYPE=htons((x==4)? IP4:IP6);
    q->QCLASS=htons(1);
}
void affiche_header(DNS_h* header,int i){
        switch(i){
            case 0  : if (ntohs(header->RD) !=0) printf(" rd "); return;
            case 1  : if (ntohs(header->TC) !=0) printf(" tc "); return;
            case 2  : if (ntohs(header->AA) !=0) printf(" aa "); return;
            case 3  : if (ntohs(header->OPCODE) !=0) printf(" opcode : %d ,",header->OPCODE); return;
            case 4  : if (ntohs(header->QR) !=0) printf(" qr "); return;
            case 5  : if (ntohs(header->RCODE) !=0) printf(" rcode : %d ,",header->RCODE); return;
            case 6  : if (ntohs(header->RA) !=0) printf(" ra "); return;
            default :
                return;
        }
        return;
}

void answer(unsigned char * query ,unsigned int * offset , int count,char* s){
    int f =(s[0]==81); 
    // pour chaque réponse/Query
    for(int qw=0;qw<count;qw++){
        printf("\n============================ %s %s : %d ===================\n",(f)?"Send":"Got",s,qw+1);
        printf("%s ",(f)?"Search For :":"For : ");

         // affichage du domaine name, type et classe
        *offset += Namehere(query,*offset,0,0);
        DNS_ANS* an =(DNS_ANS*)&query[*offset];
        affques( (DNS_q* )an);
        if (f) {
            *offset+=sizeof(DNS_q);continue;
        }

        *offset+=sizeof(DNS_ANS);
        // Durée du cache DNS
        printf(";; Time to Live : %d Sec \n",(ntohs(an->Ttl)*65536) + ntohs(an->Ttl2));
        unsigned int Data_l = ntohs(an->DA_L);
        unsigned char  data[Data_l];
        memcpy(data,query+(*offset),Data_l);

        // determination du type de réponse et affichage
        switch(ntohs(an->Type)){
            case IP4 : affipv4(data); break;
            case IP6 : affipv6(data); break;
            case SOA : printf(";; Primary name Server : "); Namehere(query,*offset,0,0) ;break;
            case C_N : printf(";; Canonical Name : ") ; Namehere(query,*offset,0,0) ;break;
            case N_S : printf(";; Authoritative name Server : "); Namehere(query,*offset,0,0) ;break;
            default: break;
        }
        *offset+=Data_l; 
            
    }
}

void makeDomaine(char* tab,char*dom){
    char * tok ;
    int j=0;
    while( (tok = strsep(&dom,".")) != NULL ){
        int x =strlen(tok);
        tab[j++]=x;
        strcpy(tab+j,tok);
        j+=x;
    }

}

int main(int argc,char** argv){
    // Si les paramètres sont invalides
    if (argc==1) {
        // affichage de l'utilisation de la commande
        printf("Usage:\n%s DOMAINE_NAME IPformat(4/6) DNS_SERVER (9.9.9.9 by default) \nDNS_SERVER format : @IP or Server_Name\n",argv[0]);
        // code d'erreur
        exit(EXIT_FAILURE);
    }

    ssize_t x=0;
    // constante initalisée dans head.h par defaut 9.9.9.9
    char* dnsse = DNS_SERVER;
// case of querying a specific DNS Server instead of Quad9 
// either his name or his IP@
    if (argc== 4){
        if(argv[3][0] == 64) dnsse = &argv[3][1];
        else{
            // duplciation du processus avec fork()
            switch(fork()){
                // code d'erreur
                case -1 : exit(EXIT_FAILURE);
                // processus fils de fork
                case 0  :execlp("./dns","./dns",argv[3],"4",NULL); // nouvelle image du processus de la commande avec ses arguments
                default :
                    printf("Resolving DNS Server Name ...\n");
                    wait(NULL);
                    // ouverture du fichier de sortie
                    FILE* fd = fopen("ip.txt","r");
                    check(fd,0,"IP not fetched");
                    getline(&dnsse, &x, fd);
            }

        } 
    }
    // affichage du serveur DNS questionné
    printf("DNS_SERVER TO QUERY : %s\n",dnsse);

    struct sockaddr_in soc_s , soc_r;
    int soc;
    char domaine[strlen(argv[1])+1] ;
    char query[MAX_QUERY_SIZE];

    // création du header DNS
    DNS_h header;
    
    // création de la query DNS
    DNS_q question;
    // taille du header
    ssize_t h_len = sizeof(DNS_h);
    
    // taille de la question
    ssize_t q_len = sizeof(DNS_q);

    // Set all fields to zeros 
    memset( query,0, MAX_QUERY_SIZE );
    memset(&header,0,sizeof(DNS_h));
    memset(&question,0,q_len);

    // Standart DNS domaine name format
    makeDomaine(domaine,argv[1]);
    // taille après normalisation 
    ssize_t d_len = sizeof(domaine);

    // initialisation de l'entête DNS
    init_header(&header);

    // convertion du paramètre ipv4 ou ipv6 en int
    int t= strtol(argv[2],NULL,10);
    init_question(&question,t);

    // copie du header dans la query
    memcpy(query,&header,h_len);
    
    // copie du domaine name dans la query
    memcpy(query+h_len,domaine,d_len);
    
    // copie de la question dans la query
    memcpy(query+h_len+d_len+1,&question,q_len);

    // initialisation de la famille du protocol utilisé ( ici AF_INET )
    soc_s.sin_family = AF_INET;
    
    // initialisation du port DNS ( DNS_port = 53 pour le DNS )
    soc_s.sin_port = htons(DNS_PORT);
    
    // initialisation de l'adresse du serveur DNS (inet_addr permet de mettre au format ipv4)
    soc_s.sin_addr.s_addr = inet_addr(dnsse); 
    
    fprintf(stderr, " creation du socket en mode DGRAM (UDP) ... ");
    
    // création du socket UDP
    soc = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);
    
    check(soc,-1,"socket fails");
    fprintf(stderr, "[ok]\n");

    
    unsigned long size;
    
    fprintf(stderr, " envoi du message ... ");
    
    // envoi de la query en UDP , récuperation de la longueur du message envoyé
    size = sendto(soc,query,h_len+d_len+q_len+1,0,(struct sockaddr*)&soc_s,sizeof(struct sockaddr) );
    
    // si la longueur est négative alors l'envoi a échoué
    check(size,-1,"send fails");
    fprintf(stderr, "[ok]\n longueur du message envoye : %lu\n", size);

    // initialisation du message réçu
    socklen_t rec =0;
    fprintf(stderr, " reception du message ... ");
    
    // récuperation de la longueur du message reçu
    size=recvfrom(soc, query, MAX_QUERY_SIZE, 0,(struct sockaddr *) &soc_r, &rec);
    
    // si la longueur est négative alors la reception a echouée
    check(size,-1,"recvfrom fails");
    fprintf(stderr, "[ok]\n longueur du message recu : %lu\n\n", size);
    
    // fermeture de la socket UDP
    close(soc);
    
    // Answer Section //
// initialisation du header de la réponse
    DNS_h* responseh = (DNS_h*)&query;

// header activated flags
    printf(";; Header flags :");
    // affichage de l'entête
    
    // pour tous les flags
    for (int dds=0;dds<7;dds++) affiche_header(responseh,dds);
    
    unsigned int tab[4];
    
    // convertion des byte en décimal avec ntohs()
    tab[0]= ntohs(responseh->QDCOUNT);
    tab[1]= ntohs(responseh->ANSCOUNT);
    tab[2]= ntohs(responseh->AUTH_RRS);
    tab[3] = ntohs(responseh->ADD_RRS);
    
    // affichage des flags
    printf("\n; nb of queries : %d\n",tab[0]);
    printf("; nb of answers : %d\n",tab[1]);
    printf("; Authorities : %d\n",tab[2]);
    printf("; Additionls : %d\n",tab[3]);
    printf("=============================================================\n");
    int offset = h_len;
    
    answer(query,&offset,tab[0],"Query ");

    answer(query,&offset,tab[1],"Answer ");
    
    answer(query,&offset,tab[2],"Authority ");
    
    answer(query,&offset,tab[3],"Additional ");
     
    exit(EXIT_SUCCESS);
}
