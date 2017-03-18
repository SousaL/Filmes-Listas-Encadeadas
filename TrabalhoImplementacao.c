//-----------------------------------------------------------------------------------------------------------------//
#include <stdio.h>
#include <locale.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <string.h>
#define THTML 5000000
#define TNOME 1000
#define TARQUIVO 1000
//------------------------------------------------------------------------------------- Autor: Leonardo P. S. Sousa//
void ToUpper(char *nome){	//FUNÇÃO PARA DEIXAR TODA ENTRADA DE STRINGS EM MAIUSCULA
	char *p = nome;
	while(*p){
		if(*p >= 97 && *p <= 122)
			*p -= 32;
		else
		switch(*p){
			case 'á':
			case 'À':
			case 'â':
			case 'Â':
			case 'ã':
			case 'Ã':
				*p = 'A';
				break;
			case 'é':
			case 'É':
			case 'ê':
			case 'Ê':
				*p = 'E';
				break;
			case 'í':
			case 'Í':
				*p = 'I';
				break;		
			case 'ô':
			case 'Ô':
			case 'ó':
			case 'Ó':
			case 'õ':
			case 'Õ':
				*p = 'O';
				break;	
			case 'ú':
				*p = 'U';
			case 'ç':
			case 'Ç':
				*p = 'C';
				break;	
		}
		*p++;
	}
}
//-----------------------------------------------------------------------------------------------------------------//
void eliminaExcesso(char *nome, int q){
	while(*(nome+q) != '\0'){
		*(nome) = *(nome + q);
		nome++;
	}
	*nome = '\0';
}

void removeMacadores(char *nome){
	char *tmp = nome;
	int cont = 0;
	while(*tmp != '\0'){
		if(*tmp != '<'){
			tmp++;
			continue;
		}
			
				
		char *tmp2 = tmp;
		cont = 0;
		while(*tmp2++ != '>')
			cont++;
		*tmp++ = ' ';
		eliminaExcesso(tmp, cont);
	}	
}
void removeQuebraLinhas(char *nome){
	char *tmp = nome;
	int cont = 0;
	while(*tmp != '\0'){
		if(*tmp != '\n'){
			tmp++;
			continue;
		}
			
				
		char *tmp2 = tmp + 1;
		cont = 0;
		while(*tmp2 == '\n' || *tmp2 == ' '){
			cont++;
			tmp2++;
		}
		if(cont > 0)
			eliminaExcesso(tmp, cont);
		*tmp = '\n';	
		tmp++;
	}
}
void limpaStr(char *str){
	char *tmp = str;
	while(*tmp != '\0'){
		if(*tmp == -95)
			*tmp = 'á';
		else if(*tmp == -87)
			*tmp = 'é';
		else if(*tmp == -77)
			*tmp = 'ó';
		else if(*tmp == -89)
			*tmp = 'ç';
		else if(*tmp == -93)
			*tmp = 'ã';
		else if(*tmp == -94)
			*tmp = 'â';
		else if(*tmp == -70)
			*tmp = 'ú';
		else if(*tmp == -83)
			*tmp = 'í';
		else if(*tmp ==  -61)	
			eliminaExcesso(tmp,1);
		else 
			tmp++;
	}	
}

int internetPersonalidade(char *nomeFilme){
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char *message = (char*)malloc(sizeof(char)*1000);			//MENSAGEM ENVIADA 	AO SERVIDOR
	char *server_reply = (char*)malloc(sizeof(char) * THTML); //MENSAGEM RECEBIDA DO SERVIDOR
    int recv_size;	//VARIAVEL DE CONTROLE COM O TAMANHO RECEBIDO;
    char *hostname = "www.adorocinema.com";	//FONTE DE DADOS
    char ip[100];
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    
    char *busca = (char*)malloc(sizeof(char) * TNOME);
	strcpy(busca,nomeFilme);
    char *p_c = busca; // PONTEIRO PRA CONTROLE
    
//    printf("Digite o nome do Filme: ");
//    fgets(busca,1000,stdin);
    while(*p_c != '\n' && *p_c != '\0'){
    	if(*p_c == 32)
    		*p_c = 43;
    	p_c++;
	}
	*p_c = '\0';
    
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
		return 0;
	
    if((he = gethostbyname(hostname )) == NULL)
        return 0;
    addr_list = (struct in_addr **) he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) 
    	strcpy(ip , inet_ntoa(*addr_list[i]) );
	
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
        return 0;
    
	server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( 80 );
 
    if (connect(s, (struct sockaddr *)&server ,sizeof(server)) < 0)
        return 0;
    strcpy(message,"GET /busca/5/?q=");
    strcat(message, busca);
    strcat(message, " HTTP/1.1\nHost: www.adorocinema.com\r\n\r\n");
//	printf("%s\n", message);
    if( send(s , message , strlen(message) , 0) < 0)
        return 0;
    if((recv_size = recv(s , server_reply , THTML , 0)) == SOCKET_ERROR)
        return 0;
    server_reply[recv_size] = '\0';
//	system("pause"); printf(server_reply);system("pause");
	if(strstr(server_reply,"\n0 resultado encontrado nos n") != NULL || strstr(server_reply,"Bad Request") != NULL)
	{
		printf("Personalidade nao encontrada\n");
		return 0;
	}else{
		printf("");
	}
	char *idF = (char*)malloc(sizeof(char)*100);
	
	strncpy(idF, strstr(strstr(server_reply,"Estrelas</h2>"), "personalidades/personalidade-") + 29, strchr(strstr(strstr(server_reply,"Estrelas</h2>"), "personalidade-"),'/') - (strstr(strstr(server_reply,"Estrelas</h2>"), "personalidades/personalidade-") + 29));
	idF[strchr(strstr(strstr(server_reply,"Estrelas</h2>"), "personalidade-"),'/') - (strstr(strstr(server_reply,"Estrelas</h2>"), "personalidades/personalidade-") + 29)] = '\0';
//	printf("\n%s",idF);
//	return 0;
	strcpy(message,"GET /personalidades/personalidade-");
	strcat(message,idF);
	strcat(message,"/biografia/ HTTP/1.1\nHost: www.adorocinema.com\r\n\r\n");
	
    if( send(s , message , strlen(message) , 0) < 0)
        return 0;
   	char *html = (char*)malloc(sizeof(char) * THTML);
   	
	while(1){
	    if((recv_size = recv(s , server_reply , THTML , 0)) == SOCKET_ERROR)
        	return 0;
	    server_reply[recv_size] = '\0';
		strcat(html, server_reply);
		if(strstr(html,"</html>") != NULL)
			break;	
   	}
//   	char *tmp = (char*)malloc(sizeof(char) * THTML);
//   	memcpy(html,strstr(strstr(html,"< module/header/mybrand.html.twig -->"),"<ul class=\"list_item_p2v tab_col_first\">"),500);
//	html[500] = '\0';
	if(strstr(html,"<!--/tt_ribbon-->") != NULL)
		*strstr(html,"<!--/tt_ribbon-->") = '\0';
	limpaStr(html);
   	removeMacadores(html);
   	removeQuebraLinhas(html);
	printf("%s",strstr(html,"sicas") + 6);
//	free(sinopse);
	free(html);
	return 0;
//	while(*(tmp++) != '\0')
//		if(*tmp < 0)
//			printf("%c = %d\n", *tmp, *tmp); 
	return 1;
}
int internet(char *nomeFilme){
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char *message = (char*)malloc(sizeof(char)*1000);			//MENSAGEM ENVIADA 	AO SERVIDOR
	char *server_reply = (char*)malloc(sizeof(char) * THTML); //MENSAGEM RECEBIDA DO SERVIDOR
    int recv_size;	//VARIAVEL DE CONTROLE COM O TAMANHO RECEBIDO;
    char *hostname = "www.adorocinema.com";	//FONTE DE DADOS
    char ip[100];
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    
    char *busca = (char*)malloc(sizeof(char) * TNOME);
	strcpy(busca,nomeFilme);
    char *p_c = busca; // PONTEIRO PRA CONTROLE
    
//    printf("Digite o nome do Filme: ");
//    fgets(busca,1000,stdin);
    while(*p_c != '\n' && *p_c != '\0'){
    	if(*p_c == 32)
    		*p_c = 43;
    	p_c++;
	}
	*p_c = '\0';
    
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
		return 0;
	
    if((he = gethostbyname(hostname )) == NULL)
        return 0;
    addr_list = (struct in_addr **) he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) 
    	strcpy(ip , inet_ntoa(*addr_list[i]) );
	
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
        return 0;
    
	server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( 80 );
 
    if (connect(s, (struct sockaddr *)&server ,sizeof(server)) < 0)
        return 0;
    strcpy(message,"GET /busca/1/?q=");
    strcat(message, busca);
    strcat(message, " HTTP/1.1\nHost: www.adorocinema.com\r\n\r\n");
//	printf("%s\n", message);
    if( send(s , message , strlen(message) , 0) < 0)
        return 0;
    if((recv_size = recv(s , server_reply , THTML , 0)) == SOCKET_ERROR)
        return 0;
    server_reply[recv_size] = '\0';
//	system("pause"); printf(server_reply);system("pause");
	if(strstr(server_reply,"\n0 resultado encontrado nos t") != NULL || strstr(server_reply,"Bad Request") != NULL)
	{
		printf("Filme nao encontrado\n");
		return 0;
	}else{
		printf("");
	}
	char *idF = (char*)malloc(sizeof(char)*100);
	
	strncpy(idF, strstr(strstr(server_reply,"Filmes</h2>"), "filmes/filme-") + 13, strchr(strstr(strstr(server_reply,"Filmes</h2>"), "filme-"),'/') - (strstr(strstr(server_reply,"Filmes</h2>"), "filmes/filme-") + 13));
	idF[strchr(strstr(strstr(server_reply,"Filmes</h2>"), "filme-"),'/') - (strstr(strstr(server_reply,"Filmes</h2>"), "filmes/filme-") + 13)] = '\0';
	strcpy(message,"GET /filmes/filme-");
	strcat(message,idF);
	strcat(message,"/ HTTP/1.1\nHost: www.adorocinema.com\r\n\r\n");
	
    if( send(s , message , strlen(message) , 0) < 0)
        return 0;
   	char *html = (char*)malloc(sizeof(char) * THTML);
   	
	while(1){
	    if((recv_size = recv(s , server_reply , THTML , 0)) == SOCKET_ERROR)
        	return 0;
	    server_reply[recv_size] = '\0';
		strcat(html, server_reply);
		if(strstr(html,"</html>") != NULL)
			break;	
   	}
   	
   	char *sinopse = (char*)malloc(sizeof(char) * 5000);
	strncpy(sinopse,strstr(strstr(html,"<section class=\"section ovw ovw-synopsis\">"),"itemprop=\"description\">") + 24, strstr(strstr(strstr(html,"<section class=\"section ovw ovw-synopsis\">"),"itemprop=\"description\">"),"</div>") - (strstr(strstr(html,"<section class=\"section ovw ovw-synopsis\">"),"itemprop=\"description\">") + 24));
   	limpaStr(sinopse);
	if(strstr(sinopse,"<p>") == NULL)
		printf("%s", sinopse);
	else{
		*strstr(sinopse,"</p>") = '\0';
		printf("%s", strstr(sinopse,"<p>") + 3);
		
	}
	free(sinopse);
	free(html);
//	while(*(tmp++) != '\0')
//		if(*tmp < 0)
//			printf("%c = %d\n", *tmp, *tmp); 
	return 1;
}
//-----------------------------------------------------------------------------------------------------------------//
struct nodoFilmes;
struct nodoCadastro;
struct nodoFilmesSimples;
struct nodoCadastroSimples;
//-----------------------------------------------------------------------------------------------------------------//
typedef struct nodoFilmes nodoFilmes;
typedef struct nodoCadastro nodoCadastro;
//-----------------------------------------------------------------------------------------------------------------//
typedef struct nodoFilmesSimples nodoFilmesSimples;
typedef struct nodoCadastroAtoresSimples nodoCadastroAtoresSimples;
typedef struct nodoCadastroDiretoresSimples nodoCadastroDiretoresSimples;

void printListaPrincipalFilmes(nodoFilmes *nodo, int ordem);
void printListaPrincipalSimples(nodoCadastro *nodo, int ordem);
void printListaSimplesFilmes(nodoFilmesSimples *nodo);
void printListaSimplesAtores(nodoCadastroAtoresSimples *nodo);
void printListaSimplesDiretores(nodoCadastroDiretoresSimples *nodo);
//-----------------------------------------------------------------------------------------------------------------//
struct nodoFilmes{									//NODO PARA A LISTA DE FILMES
	char *nome;										//NOME DO RESPECTIVO FILME
	int ano;										//ANO DO FILME
	nodoCadastroDiretoresSimples *inicioDiretores;	//INICIO DE LISTA SIMPLES DE DIRETORES
	nodoCadastroAtoresSimples *inicioAtores;		//INICIO DE LISTA SIMPLES DE ATORES
	nodoFilmes *prox;								//PONTEIRO PARA O PROXIMO FILME
	nodoFilmes *ant;								//PONTEIRO PARA O FILME ANTERIOR
};
struct nodoCadastro{								//NODO DE CADASTRO SECUNDARIO(DIRETOR E ATOR)
	char *nome;										//NOME DO REGISTRO
	nodoFilmesSimples *inicioFilmes;				//INICIO DE LISTA SIMPLES DE FILMES
	nodoCadastro *prox;								//PONTEIRO PARA O PROXIMO CADASTRO
	nodoCadastro *ant;								//PONTEIRO PARA O CADASTRO ANTERIOR
};
struct nodoFilmesSimples{							//NODO PARA LISTA 'SIMPLES' DE FILMES
	nodoFilmes *filme;								//PONTEIRO PARA O RESPECTIVO FILME
	nodoFilmesSimples *prox;						//PONTEIRO PARA O PROXIMO FILME
};
struct nodoCadastroAtoresSimples{					//NODO PARA LISTA 'SIMPLES' DE ATORES
	char *personagem;								//NOME DO PERSONAGEM DO QUAL O ATOR INTERPRETOU
	nodoCadastro *ator;								//PONTEIRO PARA O REGISTRO NA LISTA PRINCIPAL DE ATORES
	nodoCadastroAtoresSimples *prox;				//PONTEIRO PARA O PROXIMO REGISTRO
};
struct nodoCadastroDiretoresSimples{				//PONTEIRO PARA LISTA 'SIMPLES' DE DIRETOR
	nodoCadastro *diretor;							//PONTEIRO PARA A LISTA PRINCIPAL DE DIRETOR
	nodoCadastroDiretoresSimples *prox;				//PONTEIRO PARA O PROXIMO REGISTRO
};
//-----------------------------------------------------------------------------------------------------------------//
nodoFilmes *alocaNodoFilmes(char *nome, int ano){	//FUNÇÃO PARA A ALOCAÇÃO DO NODO NA LISTA FILME PRINCIPAL
	nodoFilmes *pTmp = (nodoFilmes*)malloc(sizeof(nodoFilmes));
	pTmp->nome = (char*)malloc(sizeof(char) * TNOME);
	strcpy(pTmp->nome,nome);
	pTmp->ano = ano;
	pTmp->ant = pTmp->prox = NULL;
	pTmp->inicioAtores = NULL;
	pTmp->inicioDiretores = NULL;
	ToUpper(pTmp->nome);
	return pTmp;
}
nodoCadastro *alocaNodoCadastro(char *nome){	//FUNÇÃO PARA A ALOCAÇÃO DO NODO NA LISTA CADASTRO(ATOR OU DIRETOR) PRINCIPAL
	nodoCadastro *pTmp = (nodoCadastro*)malloc(sizeof(nodoCadastro));
	pTmp->nome = (char *)malloc(sizeof(char) * TNOME);
	strcpy(pTmp->nome, nome);
	pTmp->ant = pTmp->prox = NULL;
	pTmp->inicioFilmes = NULL;
	ToUpper(pTmp->nome);
	return pTmp;
}
nodoFilmesSimples *alocaListaSimplesFilmes(nodoFilmes *pFilme){ //FUNÇÃO PARA A ALOCAÇÃO DO NODO NA LISTA FILME 'SIMPLES'
	nodoFilmesSimples *pTmp = (nodoFilmesSimples*)malloc(sizeof(nodoFilmesSimples));
	pTmp->filme = pFilme;
	pTmp->prox = NULL;
	return pTmp;
}
nodoCadastroAtoresSimples *alocaListaSimplesAtores(char *nome, nodoCadastro *pAtor){ //FUNÇÃO PARA A ALOCAÇÃO DO NODO NA LISTA ATORES 'SIMPLES'
	nodoCadastroAtoresSimples *pTmp = (nodoCadastroAtoresSimples*)malloc(sizeof(nodoCadastroAtoresSimples));
	pTmp->personagem = (char*)malloc(sizeof(char) * TNOME);
	strcpy(pTmp->personagem, nome);
	pTmp->prox = NULL;
	pTmp->ator = pAtor;	
}
nodoCadastroDiretoresSimples *alocaListaSimplesDiretores(nodoCadastro *pDiretor){	//FUNÇÃO PARA A ALOCAÇÃO DO NODO NA LISTA DIREOTRES 'SIMPLES'
	nodoCadastroDiretoresSimples *pTmp = (nodoCadastroDiretoresSimples *)malloc(sizeof(nodoCadastroDiretoresSimples));
	pTmp->prox = NULL;
	pTmp->diretor = pDiretor;
	return pTmp;
}
//-----------------------------------------------------------------------------------------------------------------//
//FUNÇÕES PARA VERIFICAR SE O NODO QUE SE PRETENTE ADICIONAR JÁ EXISTE, TENDO ASSIM O RETORNO CASO ELE EXISTA CASO
//CONTRÁRIO TERÁ O RETORNO COMO NULL

nodoFilmes *contemFilme(nodoFilmes *nodo, char *nome, int ano){
	if(nodo == NULL || (strcmp(nodo->nome, nome) == 0 && nodo->ano == ano))
		return nodo;
	return contemFilme(nodo->prox, nome, ano);
}
nodoCadastro *contemAtor(nodoCadastro *nodo, char *nome){
	if(nodo == NULL || strcmp(nodo->nome, nome) == 0)
		return nodo;
	return contemAtor(nodo->prox, nome);
}
nodoFilmesSimples *contemFilmesSimples(nodoFilmesSimples *nodo, nodoFilmes *nodoNovo){
	if(nodo == NULL || nodo->filme == nodoNovo)
		return nodo;
	return contemFilmesSimples(nodo->prox, nodoNovo);
}
nodoCadastroAtoresSimples *contemAtoresSimples(nodoCadastroAtoresSimples *nodo, nodoCadastro *nodoNovo){
	if(nodo == NULL || nodo->ator == nodoNovo)
		return nodo;
	return contemAtoresSimples(nodo->prox, nodoNovo);
}

nodoCadastroDiretoresSimples *contemDiretoresSimples(nodoCadastroDiretoresSimples *nodo, nodoCadastro *nodoNovo){
	if(nodo == NULL || nodo->diretor == nodoNovo)
		return nodo;
	return contemDiretoresSimples(nodo->prox, nodoNovo);
}
//-----------------------------------------------------------------------------------------------------------------//
//FUNÇÕES PARA A INSERÇÃO NAS LISTAS EXISTENTES, O REGISTRO JÁ É FEITO UTILIZANDO A ALOCAÇÃO DINÂMICA E TAMBEM
//INSERINDO ORDENAMENTE
nodoFilmes *insereFilmeOrdenado(nodoFilmes **pLista, char *nome, int ano){
	nodoFilmes *novoFilme = alocaNodoFilmes(nome, ano);
	nodoFilmes **aux = pLista;
	
	if((*aux) == NULL){
		novoFilme->prox = (*pLista);
		*pLista = novoFilme;	
	}
	else{
		while((*aux)->prox != NULL && strcmp((*aux)->prox->nome, nome) < 0)
			aux = &(*aux)->prox;
		
		novoFilme->prox = (*aux)->prox;
		if((*aux)->prox != NULL)
			(*aux)->prox->ant = novoFilme;
		(*aux)->prox = novoFilme;
		novoFilme->ant = (*aux);
	}
	return novoFilme;
}
nodoCadastro *insereSimplesOrdenado(nodoCadastro **pLista, char *nome){
	nodoCadastro *tmp = contemAtor(*pLista, nome);
	if(tmp != NULL)
		return tmp;
	nodoCadastro *novoCadastro = alocaNodoCadastro(nome);
	nodoCadastro **aux = pLista;
	
	if((*aux) == NULL || strcmp((*aux)->nome, nome) > 0){
		novoCadastro->prox = (*pLista);
		*pLista = novoCadastro;
	}
	else{
		while((*aux)->prox != NULL && strcmp((*aux)->prox->nome, nome) < 0)
			aux = &(*aux)->prox;
		
		novoCadastro->prox = (*aux)->prox;
		if((*aux)->prox != NULL)
			(*aux)->prox->ant = novoCadastro;
		(*aux)->prox = novoCadastro;
		novoCadastro->ant = (*aux);
	}
	return novoCadastro;
}
nodoFilmesSimples *insereFilmeSimples(nodoFilmesSimples **pLista, nodoFilmes *filme){
	nodoFilmesSimples *tmp = contemFilmesSimples((*pLista), filme);
	if(tmp != NULL)
		return tmp;
	nodoFilmesSimples *novoFilme = alocaListaSimplesFilmes(filme);
	nodoFilmesSimples **aux = pLista;
	
	if((*aux) == NULL || ((*aux)->filme->ano > filme->ano && filme->ano != 0)){
		novoFilme->prox = *pLista;
		*pLista = novoFilme;	
	}
	else if(filme->ano == 0){
		while((*aux)->prox != NULL)
			aux = &(*aux)->prox;
		return (*aux)->prox = novoFilme;
	}
	else{
		while((*aux)->prox != NULL && (*aux)->prox->filme->ano < filme->ano && (*aux)->prox->filme->ano != 0)
			aux = &(*aux)->prox;
		
		novoFilme->prox = (*aux)->prox;
		(*aux)->prox = novoFilme;
	}
	return novoFilme;
}
nodoCadastroAtoresSimples *insereAtoresSimples(nodoCadastroAtoresSimples **pLista, nodoCadastro *ator, char *nome){
	nodoCadastroAtoresSimples *tmp = contemAtoresSimples(*pLista, ator);
	if(tmp != NULL)
		return tmp;
	nodoCadastroAtoresSimples *novoAtor = alocaListaSimplesAtores(nome, ator);
	nodoCadastroAtoresSimples **aux = pLista;
	
	if((*aux) == NULL || strcmp((*aux)->ator->nome, ator->nome) > 0){
		novoAtor->prox = *pLista;
		*pLista = novoAtor;	
	}
	else{
		while((*aux)->prox != NULL && strcmp((*aux)->prox->ator->nome, ator->nome) < 0)
			aux = &(*aux)->prox;
		
		novoAtor->prox = (*aux)->prox;
		(*aux)->prox = novoAtor;
	}
	
	return novoAtor;
}
nodoCadastroDiretoresSimples *insereDiretoresSimples(nodoCadastroDiretoresSimples **pLista, nodoCadastro *diretor){
	nodoCadastroDiretoresSimples *tmp = contemDiretoresSimples(*pLista, diretor);
	if(tmp != NULL)
		return tmp;
	nodoCadastroDiretoresSimples *novoDiretores = alocaListaSimplesDiretores(diretor);
	nodoCadastroDiretoresSimples **aux = pLista;
	
	if((*aux) == NULL || strcmp((*aux)->diretor->nome, diretor->nome) > 0){
		novoDiretores->prox = *pLista;
		*pLista = novoDiretores;	
	}
	else{
		while((*aux)->prox != NULL && strcmp((*aux)->prox->diretor->nome, diretor->nome) < 0)
			aux = &(*aux)->prox;
		
		novoDiretores->prox = (*aux)->prox;
		(*aux)->prox = novoDiretores;
	}
	
	return novoDiretores;
}
//-----------------------------------------------------------------------------------------------------------------//
void excluirFime(nodoFilmes **pF,nodoCadastro **pA, nodoCadastro **pD, nodoFilmes *filme){
	//EXCLUSÃO POR PARTES
	//DIRETOR(ES)
	nodoCadastroDiretoresSimples *diretor_aux = filme->inicioDiretores;
	nodoFilmesSimples **filme_aux;
	while(diretor_aux != NULL){
		filme_aux = &(diretor_aux->diretor->inicioFilmes);
		while(*filme_aux != NULL){
			if((*filme_aux)->filme == filme){
				nodoFilmesSimples *filme_tmp = *filme_aux;
				*filme_aux = (*filme_aux)->prox;
				free(filme_tmp);
				break;
			}
			filme_aux = &((*filme_aux)->prox);
		}
		if(diretor_aux->diretor->inicioFilmes == NULL){//ENTÃO NAO SOBROU NENHUM FILME 
			nodoCadastro **pD_aux = pD;
			nodoCadastro *tmp = *pD;
			while(*pD_aux != NULL){
				if(*pD_aux != diretor_aux->diretor){
					pD_aux = &((*pD_aux)->prox);
					continue;
				}
				tmp = *pD_aux;
				if((*pD_aux)->ant != NULL)
					(*pD_aux)->ant->prox = (*pD_aux)->prox;
				if((*pD_aux)->prox != NULL)
					(*pD_aux)->prox->ant = (*pD_aux)->ant;
				if(*pD = tmp)
					*pD = (*pD)->prox;
			}			
			free(tmp->nome);
			free(tmp->inicioFilmes);
			free(tmp);
		}
		nodoCadastroDiretoresSimples *diretor_tmp = diretor_aux;
		diretor_aux = diretor_aux->prox;
		free(diretor_tmp);
	}
	nodoCadastroAtoresSimples *ator_aux = filme->inicioAtores;
	while(ator_aux != NULL){
		filme_aux = &(ator_aux->ator->inicioFilmes);
		while(*filme_aux != NULL){
			if((*filme_aux)->filme == filme){
				nodoFilmesSimples *filme_tmp = *filme_aux;
				*filme_aux = (*filme_aux)->prox;
				free(filme_tmp);
				break;
			}
			filme_aux = &((*filme_aux)->prox);
		}
		if(ator_aux->ator->inicioFilmes == NULL){
			free(ator_aux->ator->nome);
			free(ator_aux->ator->inicioFilmes);
			
			nodoCadastro *ator_tmp = ator_aux->ator;
			
			if(ator_aux->ator->ant != NULL)
				ator_aux->ator->ant = ator_aux->ator->prox;
			if(ator_aux->ator->prox != NULL)
				ator_aux->ator->prox = ator_aux->ator->ant;
			if((*pA) == ator_tmp)
				(*pA) == ator_aux->ator;
			ator_aux->ator = ator_aux->ator->prox;
			free(ator_tmp);
		}
		free(ator_aux->personagem);
		nodoCadastroAtoresSimples *ator_tmp = ator_aux;
		ator_aux = ator_aux->prox;
		free(ator_tmp);
	}
	free(filme->nome);
	nodoFilmes *filme_tmp = filme;
	if(filme_tmp->prox != NULL)
		filme_tmp->prox->ant = filme->ant;
	if(filme_tmp->ant != NULL)
		filme_tmp->ant->prox = filme->prox;
	if((*pF) == filme)
		*pF = (*pF)->prox;
	free(filme);
}
//-----------------------------------------------------------------------------------------------------------------//
//PRINT PADRÕES DAS LISTAS EXISTENTES, CADA FUNÇÃO RECEBE UM PONTEIRO INICIAL DA RESPECTIVA LISTA E SERÁ EXIBIDO 
//NA TELA
void printListaPrincipalFilmes(nodoFilmes *nodo, int ordem){
	if(nodo == NULL)
		return;
	if(ordem){
		printListaPrincipalFilmes(nodo->prox, ordem);	
		printf("+----+------------------------------------------------------------+\n");
		printf("|%04d|%-60s|\n", nodo->ano, nodo->nome);
//		printf("+----+------------------------------------------------------------+\n");
	}	
	else{
		printf("+----+------------------------------------------------------------+\n");
		printf("|%04d|%-60s|\n", nodo->ano, nodo->nome);
//		printf("+----+------------------------------------------------------------+\n");
		printListaPrincipalFilmes(nodo->prox, ordem);	
	}
}
nodoFilmes *printFilmesBusca(nodoFilmes *nodo, char *nome){
	if(nodo == NULL)
		return NULL;
	nodoFilmes *aux = nodo;
	nodoFilmes *tmp = NULL;
	int count = 10, count_e = 0;
	while(aux != NULL)
	{
		if(count > 0 && strcmp(nome,aux->nome) == 0){
			printf("+--------------------------------------------------+\n");
			printf("|%-50s|\n", aux->nome);
			printListaSimplesAtores(aux->inicioAtores);
			printListaSimplesDiretores(aux->inicioDiretores);
			return aux;
		}
		if(count > 0 && strncmp(nome,aux->nome, strlen(nome)) == 0){
			count--;
			printf("+--------------------------------------------------+\n");
			printf("|%-50s|\n", aux->nome);
			tmp = aux;
		}
		else if(count == 0 && strncmp(nome,aux->nome, strlen(nome)) == 0)
			count_e++;		
		aux = aux->prox;
	}
	if(10 - count == 1){
			printListaSimplesAtores(tmp->inicioAtores);
			printListaSimplesDiretores(tmp->inicioDiretores);
			return tmp;
	}
	if(count_e > 0)
		printf("|E mais: %04d\n", count_e);
	return NULL;
}
void printListaPrincipalSimples(nodoCadastro *nodo,int ordem){
	if(nodo == NULL)
		return;
	if(ordem){
		printListaPrincipalSimples(nodo->prox, ordem);
		printf("+------------------------------------------------------------+\n");
		printf("|%-60s|\n",nodo->nome);
	}
	else{
		printf("+------------------------------------------------------------+\n");
		printf("|%-60s|\n", nodo->nome);
		printListaPrincipalSimples(nodo->prox, ordem);
	}
}
nodoCadastro *printListaPrincipalSimplesBusca(nodoCadastro *nodo, char *nome){
	if(nodo == NULL)
		return;
	nodoCadastro *aux = nodo;
	nodoCadastro *tmp = NULL;
	int count = 10, count_e = 0;
	while(aux != NULL)
	{
		if(count > 0 && strcmp(nome,aux->nome) == 0){
			printf("+--------------------------------------------------+\n");
			printf("|%-50s|\n",aux->nome);
			printListaSimplesFilmes(aux->inicioFilmes);
			return aux;
		}
		else if(count > 0 && strncmp(nome,aux->nome, strlen(nome)) == 0){
			count--;
			printf("+--------------------------------------------------+\n");
			printf("|%-50s|\n",aux->nome);
			tmp = aux;
		}
		else if(count == 0 && strncmp(nome,aux->nome, strlen(nome)) == 0)
			count_e++;		
		aux = aux->prox;
	}
	if(10 - count == 1){
			printListaSimplesFilmes(tmp->inicioFilmes);
			return tmp;
	}
	if(count_e > 0)
		printf("|E mais: %04d\n", count_e);
	else
		while(count-- >0)
			puts("");
	return NULL;		
}
void printListaPrincipalSimplesBuscaAxA(nodoCadastro *nodo, char *nome){
	if(nodo == NULL)
		return;
	nodoCadastro *aux = nodo;
	nodoCadastro *tmp = NULL;
	int count = 10, count_e = 0;
	while(aux != NULL)
	{
		if(count > 0 && strncmp(nome,aux->nome, strlen(nome)) == 0){
			count--;
			printf("+--------------------------------------------------+\n");
			printf("|%-50s|\n",aux->nome);
			tmp = aux;
		}
		else if(count == 0 && strncmp(nome,aux->nome, strlen(nome)) == 0)
			count_e++;		
		aux = aux->prox;
	}
	if(10 - count == 1){
			nodoFilmesSimples *aux_i = tmp->inicioFilmes;
			nodoCadastroAtoresSimples *aux_j = NULL;
			while(aux_i != NULL){
				aux_j = aux_i->filme->inicioAtores;
				printf("+----+--------------------------------------------------+\n");
				printf("|%04d|%-50s|\n", aux_i->filme->ano,aux_i->filme->nome);
				while(aux_j != NULL){
					if(aux_j->ator != tmp)
						printf("     |%-25s|%-50s|\n",aux_j->personagem,aux_j->ator->nome);
					aux_j = aux_j->prox;
				}
				aux_i = aux_i->prox;
			}
			
			return;
	}
	if(count_e > 0)
		printf("|E mais: %04d\n", count_e);
		
}
void printListaPrincipalSimplesBuscaDxA(nodoCadastro *nodo, char *nome){
	if(nodo == NULL)
		return;
	nodoCadastro *aux = nodo;
	nodoCadastro *tmp = NULL;
	int count = 10, count_e = 0;
	while(aux != NULL)
	{
		if((count > 0) && strcmp(nome, aux->nome) == 0){
			nodoFilmesSimples *aux_i = aux->inicioFilmes;
			nodoCadastroAtoresSimples *aux_j = NULL;
			while(aux_i != NULL){
				aux_j = aux_i->filme->inicioAtores;
				printf("+----+--------------------------------------------------+\n");
				printf("|%04d|%-50s|\n", aux_i->filme->ano,aux_i->filme->nome);
				while(aux_j != NULL){
					if(aux_j->ator != tmp)
						printf("     |%-50s|\n",aux_j->ator->nome);
					aux_j = aux_j->prox;
				}
				aux_i = aux_i->prox;
			}
			
			return;
		}
		else if(count > 0 && strncmp(nome,aux->nome, strlen(nome)) == 0){
			count--;
			printf("+--------------------------------------------------+\n");
			printf("|%-50s|\n",aux->nome);
			tmp = aux;
		}
		else if(count == 0 && strncmp(nome,aux->nome, strlen(nome)) == 0)
			count_e++;		
		aux = aux->prox;
	}
	if(10 - count == 1){
			nodoFilmesSimples *aux_i = tmp->inicioFilmes;
			nodoCadastroAtoresSimples *aux_j = NULL;
			while(aux_i != NULL){
				aux_j = aux_i->filme->inicioAtores;
				printf("+----+--------------------------------------------------+\n");
				printf("|%04d|%-50s|\n", aux_i->filme->ano,aux_i->filme->nome);
				while(aux_j != NULL){
					if(aux_j->ator != tmp)
						printf("     |%-50s|\n",aux_j->ator->nome);
					aux_j = aux_j->prox;
				}
				aux_i = aux_i->prox;
			}
			
			return;
	}
	if(count_e > 0)
		printf("|E mais: %04d\n", count_e);
		
}
void printListaPrincipalSimplesBuscaAxP(nodoFilmes *nodo, char *nome){
	if(nodo == NULL)
		return;
	nodoFilmes *aux = nodo;
	nodoFilmes *tmp = NULL;
	nodoCadastroAtoresSimples *aux_p;
	int count = 10, count_e = 0;
	while(aux != NULL)
	{
		aux_p = aux->inicioAtores;
		while(aux_p != NULL){
			if(count > 0 && strncmp(nome,aux_p->personagem, strlen(nome)) == 0 && (*aux_p->personagem) != '\0'){
				count--;
				printf("+--------------------+--------------------------------------------------+\n");
				printf("|%-20s|%-50s|\n",aux_p->personagem,aux_p->ator->nome);
				printf("\t|Filme: %-50s|\n", aux->nome);
				tmp = aux;
			}
			else if(count == 0 && strncmp(nome,aux_p->personagem, strlen(nome)) == 0 && (*aux_p->personagem) != '\0')
				count_e++;	
			aux_p = aux_p->prox;
		}
		aux = aux->prox;
	}
	if(count_e > 0)
		printf("|E mais: %04d\n", count_e);
		
}
void printListaSimplesFilmes(nodoFilmesSimples *nodo){
	if(nodo == NULL)
		return;
//	printf("+----+------------------------------------------------------------+\n");
//	printf("|Ano | Nome do Filme                                              |\n");	
	printf("+----+------------------------------------------------------------+\n");
	printf("|%04d|%-60s|\n",nodo->filme->ano, nodo->filme->nome);
	printListaSimplesFilmes(nodo->prox);
}
void printListaSimplesAtores(nodoCadastroAtoresSimples *nodo){
	if(nodo == NULL)
		return;
	printf("+--------------------------------------------------+--------------------+\n");
//	printf("| Nome do Ator                                     | Personagem         |\n");
//	printf("+--------------------------------------------------+--------------------+\n");
	printf("|%-50s|%20s|\n", nodo->ator->nome, nodo->personagem);
	printListaSimplesAtores(nodo->prox);
}

void printListaSimplesDiretores(nodoCadastroDiretoresSimples *nodo){
	if(nodo == NULL)
		return;
	printf("+--------------------------------------------------+--------------------+\n");
	printf("|%50s| Diretor            |\n", nodo->diretor->nome);
//	printf("+--------------------------------------------------+----------+\n");
	printListaSimplesDiretores(nodo->prox);
}
//-----------------------------------------------------------------------------------------------------------------//
//FUNÇÃO PRINCIPAL NA LEITURA DE FILMES ELE ENTÃO REALIZARÁ TODO O PROCESSO, RECEBENDO O PONTEIRO PARA O CADASTRO
//DOS FILMES, DO CADASTRO, NOME DO FILME, ANO E ELENCO

void cadastroFilmeGeral(nodoFilmes **pF,nodoCadastro **pA, nodoCadastro **pD, char *nome, int ano, char *elenco){
	nodoFilmes *novoFilme = insereFilmeOrdenado(pF,nome,ano);
	//ALOCA O NOVO FILME E JÁ GUARDA EM UMA VARIAVEL O ENDERECO
	nodoCadastroDiretoresSimples **dTmp = (nodoCadastroDiretoresSimples**)malloc(sizeof(nodoCadastroDiretoresSimples*));
	nodoCadastroAtoresSimples **aTmp = (nodoCadastroAtoresSimples**)malloc(sizeof(nodoCadastroAtoresSimples*));
	//CRIA LISTAS SIMPLES TANTO DIRETOR E ATOR SIMPLES PARA O FILME SE VINCULAR
	*aTmp = NULL;
	*dTmp = NULL;
	char *tmp = (char*)malloc(sizeof(char)*TARQUIVO), *novaP = NULL;
	int x = 0;
	strcpy(tmp,elenco);
	if(*tmp != ',')			//quer dizer que tem diretor
		while(*tmp != '\0' && *tmp != ','){
			novaP = (char*)malloc(sizeof(char)*TNOME);
			*novaP = '\0';
			while(*tmp != '\0' && *tmp != ',')
				strncat(novaP,tmp++,1);
			insereFilmeSimples(&(insereDiretoresSimples(dTmp,insereSimplesOrdenado(pD,novaP))->diretor->inicioFilmes),novoFilme);
			free(novaP);
		}
	if(*tmp == ',')
		tmp++;	
	if(*tmp != ',')
		while(*tmp != '\0' && *tmp != ','){
				novaP = (char*)malloc(sizeof(char)*TNOME);
				*novaP = '\0';
				while(*tmp != '\0' && *tmp != ',')
					strncat(novaP,tmp++,1);
				if(memcmp(novaP,"dir:",4) == 0)
					insereFilmeSimples(&(insereDiretoresSimples(dTmp,insereSimplesOrdenado(pD,novaP+4))->diretor->inicioFilmes),novoFilme);
				else{
					if(strchr(novaP,'=') != NULL){
						char *nomeTmp = (char *)malloc(sizeof(char) * TNOME);
						int a = strchr(novaP,'=')- novaP;
						memcpy(nomeTmp,novaP,a); 
						nomeTmp[a] = '\0';
						insereFilmeSimples(&((insereAtoresSimples(aTmp,insereSimplesOrdenado(pA,nomeTmp),strchr(novaP,'=') + 1))->ator->inicioFilmes),novoFilme);
						free(nomeTmp);
					}		
					else
						insereFilmeSimples(&((insereAtoresSimples(aTmp,insereSimplesOrdenado(pA,novaP),""))->ator->inicioFilmes),novoFilme);
				}
				
				free(novaP);
					
				if(*tmp == '\0')
					break;
				tmp++;
			}	
	novoFilme->inicioAtores = *aTmp;
	novoFilme->inicioDiretores = *dTmp;		
}
//-----------------------------------------------------------------------------------------------------------------//
void leArquivo(nodoFilmes **pListaFilmes, nodoCadastro **pListaAtores, nodoCadastro **pListaDiretores){
	//LEITURA DO ARQUIVO FONTE COM OS DADOS
	FILE *a = fopen("resumo.txt","rt");
	char *linha = (char*)malloc(sizeof(char)*TARQUIVO);
	if(a == NULL){
		printf("Arquivo nao encontrado");
		return;
	}
	while(!feof(a)){
		fgets(linha,TARQUIVO,a);
		linha[strlen(linha) - 1] = '\0';
		ToUpper(linha);
		char *nomeFilme = (char*)malloc(sizeof(char)*TARQUIVO);
		memcpy(nomeFilme,linha,strchr(linha,',')-linha);
		nomeFilme[strchr(linha,',')-linha] = '\0';
		int a = 0;
		char *ano = strchr(linha,',');
		if(*(++ano) != ',')
			a = atoi(ano);
		cadastroFilmeGeral(pListaFilmes, pListaAtores, pListaDiretores, nomeFilme, a, strchr(strchr(linha,',') + 1, ',') + 1);	
		free(nomeFilme);
	}
}
//-----------------------------------------------------------------------------------------------------------------//
char menuPrincipal(){
	char opcao = '\0';
//	while(1){	
		system("cls");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",0,"Sair");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",1,"Listar todos atores da lista");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",2,"Listar todos filmes do ator(Internet)");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",3,"Listar todos os filmes em ordem");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",4,"Inserir novos filmes");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",5,"Remover Filme");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",6,"Listar todos filmes de diretor(Internet)");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",7,"Listar atores trabalharam juntos");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",8,"Listar todos atores x diretor");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",9,"Listar filmes duplicados");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",10,"Listar todos atores fizeram personagem");
		printf("+---+-----------------------------------------+\n");
		printf("|%2X | %-40s|\n",11,"Listar todos os filmes(Internet)");
		printf("+---+-----------------------------------------+\n");
		printf("|Digite: ");
		fflush(stdin);
		opcao = getchar();
		system("cls");
//		if((opcao >= '0' && opcao <= '9') || opcao == 'a')
			return opcao;
		printf("======= Opcao Invalida! =======\n");
//	}
}
void opcao1(nodoCadastro **pLista){
	//OPCAO 1 - LISTAR TODOS OS ATORES CRESCENTE OU DECRESCENTE
	fflush(stdin);
	printf("Lista de Atores: \n");
	printf("Deseja exibir a lista em ordem decrescente (S/N)?: ");
	char c = getchar();
	system("cls");
	if(c == 'S' || c == 's')
		printListaPrincipalSimples(*pLista, 1); 
	else
		printListaPrincipalSimples(*pLista, 0);
	
	printf("Total de Atores: \n");
	printf("Precione qualquer tecla para voltar ao menu anterior.\n");
	getch();
	system("cls");
}
void opcao2(nodoCadastro **pLista){
	//OPCAO 2 - LISTAR POR BUSCA TODOS OS ATORES
	nodoCadastro *tmp = NULL;
	char *busca = (char*)malloc(sizeof(char) * 100);
	*busca = '\0';
	char c[2] = {'\0','\0'};
	while(1){
		system("cls");
		ToUpper(busca);
		tmp = printListaPrincipalSimplesBusca(*pLista,busca);
		if(tmp){
			printf("+-----------------------------------------------------------------+\n");
			printf("|        Para abrir esta(a) Personalidade pressine enter!         |\n");
			printf("+-----------------------------------------------------------------+\n");
		}
			
		printf("Pesquisa: %s", busca);
		c[0] = getch();
		if(c[0] == 8)
			busca[strlen(busca) - 1] = '\0';
		else if(c[0] == '\r'){
			if(!tmp)
				return;
			else{
				system("cls");
				printf("Filme: %-60s\n",tmp->nome);
//				printf("Ano: %04d", tmp->ano);
				printListaSimplesFilmes(tmp->inicioFilmes);
//				printListaSimplesAtores(tmp->inicioAtores);
//				printListaSimplesDiretores(tmp->inicioDiretores);
				printf("Informacoes da Internet ===========================================\n");
				internetPersonalidade(tmp->nome);
				fflush(stdin);
				printf("\n\n***Para Voltar pressione enter.\n");
				getchar();
			}	
		}
		else
			strcat(busca,c);
	}
//	free(c);
}
void opcao3(nodoFilmes **pLista){
	//OPCAO 3 - LISTA TODOS OS FILMES CRESCENTE OU DECRESCENTE
	fflush(stdin);
	printf("Lista de Filmes: \n");
	printf("Deseja exibir a lista em ordem decrescente (S/N)?: ");
	char c = getchar();
	system("cls");
	if(c == 'S' || c == 's')
		printListaPrincipalFilmes(*pLista, 1); 
	else
		printListaPrincipalFilmes(*pLista, 0);
	printf("Total de Atores: \n");
	printf("Precione qualquer tecla para voltar ao menu anterior.\n");
	getch();
	system("cls");
}
void opcao4(nodoFilmes **pF, nodoCadastro **pA, nodoCadastro **pD){
	//OPCAO 4 - CADASTRO DE UM FILME
	char *nome = (char*)malloc(sizeof(char) * 100);
	char *nomeCadastro = (char*)malloc(sizeof(char) * 100);
	char *nomePersonagem = (char*)malloc(sizeof(char) * 100);
	int ano;
	nodoCadastroDiretoresSimples **dTmp = (nodoCadastroDiretoresSimples**)malloc(sizeof(nodoCadastroDiretoresSimples*));
	nodoCadastroAtoresSimples **aTmp = (nodoCadastroAtoresSimples**)malloc(sizeof(nodoCadastroAtoresSimples*));
	*aTmp = NULL;
	*dTmp = NULL;
	char opcao;
	
	printf("CADASTRO DO FILME\n");
	printf("Digite o Nome do Filme: ");
	fflush(stdin);
	fgets(nome, 100, stdin);
	nome[strlen(nome) - 1] = '\0';
	printf("Digite o Ano (Zero para deixar nao informado):");
	scanf("%d", &ano);
	if(contemFilme(*pF, nome, ano) != NULL){
		printf("Filme ja cadastrado, operacao cancelada. Pressione qualquer tecla pra continuar:");
		getch();
		system("cls");
		return;
	}
	nodoFilmes *novoFilme = insereFilmeOrdenado(pF,nome,ano);
	diretor: printf("Deseja Cadastrar um(a) Diretor(a)? (S/N)\n");
	opcao = getch();
	if(opcao == 'N' || opcao == 'n'){
		printf("\t-(Sem Diretor)\n");
		goto ator;
	}
	printf("Digite o nome do diretor: ");
	fflush(stdin);
	fgets(nomeCadastro, 100, stdin);
	nomeCadastro[strlen(nomeCadastro) - 1] = '\0';
	insereFilmeSimples(&(insereDiretoresSimples(dTmp,insereSimplesOrdenado(pD,nomeCadastro))->diretor->inicioFilmes),novoFilme);
	goto diretor;
	ator: printf("Deseja Cadastrar um(a) Ator(a)? (S/N)\n");
	opcao = getch();
	if(opcao == 'N' || opcao == 'n'){
		printf("\t-(Sem Ator)\n");
		goto final;
	}
	fflush(stdin);
	printf("Digite o nome do Ator: ");
	fgets(nomeCadastro, 100, stdin);
	nomeCadastro[strlen(nomeCadastro) - 1] = '\0';
	printf("Deseja Cadastrar o personagem do %s? (S/N)\n", nomeCadastro);
	opcao = getch();
	if(opcao == 'N' || opcao == 'n'){
		printf("\t-(Nome do Personagem nao informado)\n");
		insereFilmeSimples(&((insereAtoresSimples(aTmp,insereSimplesOrdenado(pA,nomeCadastro),""))->ator->inicioFilmes),novoFilme);
	}else{
		printf("Digite o nome do personagem que %s interpretou: ", nomeCadastro);
		fflush(stdin);
		fgets(nomePersonagem, 100, stdin);
		nomePersonagem[strlen(nomePersonagem) - 1] = '\0';
		insereFilmeSimples(&((insereAtoresSimples(aTmp,insereSimplesOrdenado(pA,nomeCadastro),nomePersonagem))->ator->inicioFilmes),novoFilme);
	}
	goto ator;	
	final: novoFilme->inicioAtores = *aTmp;
	novoFilme->inicioDiretores = *dTmp;
}
void opcao5(nodoFilmes **pF, nodoCadastro **pD, nodoCadastro **pA){
	char *busca = (char*)malloc(sizeof(char) * 100);
	nodoFilmes *tmp = NULL;
	*busca = '\0';
	char c[2] = {'\0','\0'};
	char d;
	while(1){
		system("cls");
		ToUpper(busca);
		tmp = printFilmesBusca(*pF,busca);
		if(tmp)
			printf("Para abrir este Filme pressine enter!\n");
		printf("Pesquisa: %s", busca);
		fflush(stdin);
		c[0] = getch();
		if(c[0] == 8)
			busca[strlen(busca) - 1] = '\0';
		else if(c[0] == '\r'){
			if(!tmp)
				return;
			else{
				system("cls");
				printf("Filme: %-60s\n",tmp->nome);
				printf("Ano: %04d", tmp->ano);
				printListaSimplesAtores(tmp->inicioAtores);
				printListaSimplesDiretores(tmp->inicioDiretores);
				printf("Você deseja excluir este filme (s/n)? ");
				fflush(stdin);
				d = getchar();
				if(d == 's' || d == 'S'){
						//PROCESSO DE EXCLUSAO	
						excluirFime(pF, pA, pD, tmp);
						memset(busca,0,sizeof(char) * 100);
						return;
				}
				else
					printf("Filme nao excluido\n");
//				getchar();
			}
		}
		else
			strcat(busca,c);
	}
}
void opcao7(nodoCadastro **pLista){
	char *busca = (char*)malloc(sizeof(char) * 100);
	*busca = '\0';
	char *c = (char*)malloc(1);
	while(1){
		ToUpper(busca);
		printListaPrincipalSimplesBuscaAxA(*pLista,busca);
		printf("Pesquisa: %s", busca);
		fflush(stdin);
		*c = getch();
		system("cls");
		if(*c == 8)
			busca[strlen(busca) - 1] = '\0';
		else if(*c == '\r')
		{
			free(busca);
			return;
		}	
		else
			strcat(busca,c);
	}
	free(c);
}
void opcao8(nodoCadastro **pLista){
	char *busca = (char*)malloc(sizeof(char) * 100);
	*busca = '\0';
	char *c = (char*)malloc(1);
	while(1){
		ToUpper(busca);
		printListaPrincipalSimplesBuscaDxA(*pLista,busca);
		printf("Pesquisa: %s", busca);
		fflush(stdin);
		*c = getch();
		system("cls");
		if(*c == 8)
			busca[strlen(busca) - 1] = '\0';
		else if(*c == '\r')
			return;
		else
			strcat(busca,c);
	}
	free(c);
}
void opcao9(nodoFilmes **pF){
	if(*pF == NULL || (*pF)->prox == NULL)
		return;
	nodoFilmes *aux_i = *pF;
	nodoFilmes *aux_j = (*pF)->prox;	
	while(aux_i->prox->prox != NULL){
		aux_j = aux_i->prox;
		while(aux_j->prox != NULL){
			if(!strcmp(aux_i->nome, aux_j->nome) && aux_i->ano == aux_j->ano)
				printf("(%04d)%-50s\n", aux_i->ano, aux_i->nome);
			aux_j = aux_j->prox;
		}
		aux_i = aux_i->prox;
	}
	getchar();
}
void opcao10(nodoFilmes **pLista){
	char *busca = (char*)malloc(sizeof(char) * 100);
	*busca = '\0';
	char *c = (char*)malloc(1);
	while(1){
		ToUpper(busca);
		printListaPrincipalSimplesBuscaAxP(*pLista,busca);
		printf("Pesquisa: %s", busca);
		fflush(stdin);
		*c = getch();
		system("cls");
		if(*c == 8)
			busca[strlen(busca) - 1] = '\0';
		else if(*c == '\r')
			return;
		else
			strcat(busca,c);
	}
	free(c);
}
void opcao11(nodoFilmes **pF, nodoCadastro **pD, nodoCadastro **pA){
	char *busca = (char*)malloc(sizeof(char) * 100);
	nodoFilmes *tmp = NULL;
	*busca = '\0';
	char c[2] = {'\0','\0'};
	char d;
	while(1){
		ToUpper(busca);
		tmp = printFilmesBusca(*pF,busca);
		if(tmp){
			printf("+-----------------------------------------------------------------+\n");
			printf("|        Para abrir esta Filme pressione enter!                   |\n");
			printf("+-----------------------------------------------------------------+\n");
		}
		printf("Pesquisa: %s", busca);
		fflush(stdin);
		c[0] = getch();
		if(c[0] == 8)
			busca[strlen(busca) - 1] = '\0';
		else if(c[0] == '\r'){
			if(!tmp)
				return;
			else{
				system("cls");
				printf("Filme: %-60s\n",tmp->nome);
				printf("Ano: %04d\n", tmp->ano);
				printListaSimplesAtores(tmp->inicioAtores);
				printListaSimplesDiretores(tmp->inicioDiretores);
				printf("Sinopse da internet ======================================================\n");
				internet(tmp->nome);
				fflush(stdin);
				printf("\n\n***Para Voltar pressione enter.\n");
				getchar();
			}
		}
		else
			strcat(busca,c);
		system("cls");
	}
}
int main(){
	// MENU DE ENTRADA
	setlocale(LC_ALL, "Portuguese");
	nodoCadastro **pListaAtores = (nodoCadastro**)malloc(sizeof(nodoCadastro*));	//ALOCAÇÃO DINAMICA DE LISTA DE ATORES
	nodoFilmes **pListaFilmes = (nodoFilmes**)malloc(sizeof(nodoFilmes*));			//ALOCAÇÃO DINAMICA DE LISTA DE FILMES
	nodoCadastro **pListaDiretores = (nodoCadastro**)malloc(sizeof(nodoCadastro*));	//ALOCAÇÃO DINAMICA DE LISTA DE DIRETORES
	
	*pListaFilmes = NULL;									//LISTA ENCADEADA DE FILMES
	*pListaAtores = NULL;									//LISTA ENCADEADA DE ATORES
	*pListaDiretores = NULL;								//LISTA ENCADEADA DE DIRETORES
	
	leArquivo(pListaFilmes, pListaAtores, pListaDiretores); //FUNÇÃO PARA LEITURA DE ARQUIVO
	loop: switch(menuPrincipal()){							//FUNÇÃO DO MENU PRINCIPAL
		case '0':
			return 0;
			break;
		case '1':
			opcao1(pListaAtores);
			break;
		case '2':
			opcao2(pListaAtores);
			break;
		case '3':
			opcao3(pListaFilmes);
			break;
		case '4':
			opcao4(pListaFilmes, pListaAtores, pListaDiretores);
			break;
		case '5':
			opcao5(pListaFilmes, pListaDiretores, pListaAtores);
			break;
		case '6':
			opcao2(pListaDiretores);
			break;
		case '7':
			opcao7(pListaAtores);
			break;
		case '8':
			opcao8(pListaDiretores);
			break;
		case '9':
			opcao9(pListaFilmes);
			break;
		case 'a':
		case 'A':
			opcao10(pListaFilmes);
			break;
		case 'b':
		case 'B':
			opcao11(pListaFilmes, pListaDiretores, pListaAtores);
			break;
	}
	goto loop;
	return 0;
}
