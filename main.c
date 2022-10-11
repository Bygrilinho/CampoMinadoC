#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#define ROWMAX 30 // Numero máximo de linhas
#define COLMAX 30 // Numero máximo de colunas

#define COLOR_LIGHT_RED     "\033[1;31m"
#define COLOR_LIGHT_GREEN   "\033[1;32m"
#define COLOR_LIGHT_YELLOW  "\033[1;33m"
#define COLOR_LIGHT_BLUE    "\033[1;34m"
#define COLOR_LIGHT_MAGENTA "\033[1;35m"
#define COLOR_LIGHT_CYAN    "\033[1;36m"
#define COLOR_LIGHT_RESET   "\033[0m"

#define COLOR_RED           "\x1b[31m"
#define COLOR_GREEN         "\x1b[32m"
#define COLOR_YELLOW        "\x1b[33m"
#define COLOR_BLUE          "\x1b[34m"
#define COLOR_MAGENTA       "\x1b[35m"
#define COLOR_CYAN          "\x1b[36m"
#define COLOR_RESET         "\x1b[0m"

void start();
void primeira_jogada();
void gerar_minas();
void gerar_numeros();
void jogar();
void renderizar();
void atualizar();
void game_over();
void game_win();
void exibir_tempo();

int debug = 0; // 0 = desligado, 1 = ligado
int row, col; // Tamanho do tabuleiro
int cont_minas; // Quantidade de minas
int visivel[ROWMAX][COLMAX]; // Matriz de visibilidade
int numeros[ROWMAX][COLMAX]; // Matriz de numeros
int minas[ROWMAX][COLMAX]; // Matriz de minas
int inicio, fim, tempo, tempo_seg, tempo_min, tempo_hora; // Variaveis de tempo

int main(int argc, char *argv[]){
    system("chcp 65001"); // Para funcionar acentos
    //Definir título 
    SetConsoleTitle("Campo Minado");
    system("cls");
    if(argc > 1)
    {
        if(strcmp(argv[1], "-d") == 0)
        {
            debug = 1;
            printf("Debug ligado\n\n");
        }
    }
    start();
    return 0;
}

void start(){  
    int dificuldade = 0;
    // Zerar matrizes
    for(int i=0; i<ROWMAX; i++)
    {
        for(int j=0; j<COLMAX; j++)
        {
            visivel[i][j] = 0;
            numeros[i][j] = 0;
            minas[i][j] = 0;
        }
    }
    printf("Campo Minado\n\n");

    printf("Escolha uma dificuldade (1-3, 0 para personalizada): ");
    dificuldade:
    scanf("%d", &dificuldade);
    if(dificuldade == 1){
        row = 8;
        col = 8;
        cont_minas = 10;
    }else if(dificuldade == 2){
        row = 15;
        col = 15;
        cont_minas = 40;
    }else if(dificuldade == 3){
        row = 20;
        col = 30;
        cont_minas = 150;
    }else if(dificuldade==0){
        personalizar:
        printf("Digite o numero de linhas (min 2, max %d): ", ROWMAX);
        scanf("%d", &row);
        printf("Digite o numero de colunas (min 2, max %d): ", COLMAX);
        scanf("%d", &col);
        printf("Digite o numero de minas (min 1): ");
        scanf("%d", &cont_minas);
        if(row<2 || col<2 || row>ROWMAX || col>COLMAX){
            printf("Tamanho invalido, tente novamente.\n\n");
            goto personalizar;
        }
        // Checar se o numero de minas nao eh maior que o numero de casas
        if(cont_minas<1 || cont_minas >= row*col){
            printf("Numero de minas invalido, tente novamente.\n");
            goto personalizar;
        }
    }else{
        printf("Dificuldade invalida, escolha novamente: ");
        goto dificuldade;
    }

    // Geração do jogo
    inicio = clock(); // Iniciar o cronometro

    renderizar();
    primeira_jogada();
    gerar_numeros();
    atualizar();
    renderizar();

    while(1){
        jogar();
        atualizar();
        renderizar();
    }
}

void primeira_jogada(){
    int x, y;
    char c;
    printf("\nDigite a coordenada (x y): ");
	fflush(stdin);
    scanf("%d %d", &x, &y);
    x -= 1;
    y -= 1;
    if(x<0 || x>=col || y<0 || y>=row){
        printf("Coordenada invalida, tente novamente.\n");
        primeira_jogada();
    }
    if(visivel[y][x] == 0){
        printf("Digite a acao (r - revelar, m - marcar): ");
        fflush(stdin);
        scanf("%c", &c);
        if(c == 'r'){
            visivel[y][x] = 1;
            gerar_minas(x, y);
        }else if(c == 'm'){
            visivel[y][x] = 2;
            renderizar();
            primeira_jogada();
        }else{
            printf("Acao invalida, tente novamente.\n");
            primeira_jogada();
        }
    }
    // Checar se a coordenada ja foi marcada
    else if(visivel[y][x] == 2){
        printf("Digite a acao (r - revelar, d - desmarcar): ");
        fflush(stdin);
        scanf("%c", &c);
        if(c == 'r'){
            visivel[y][x] = 1;
            gerar_minas(x, y);
        }else if(c == 'd'){
            visivel[y][x] = 0;
            renderizar();
            primeira_jogada();
        }else{
            printf("Acao invalida, tente novamente.\n");
            primeira_jogada();
        }
    }
    else{
        // Resetar visibilidade (vai que eu fiz alguma merda)
        visivel[y][x] = 0;
        printf("Erro desconhecido, tente novamente.\n");
        primeira_jogada();
    }
}

void gerar_minas(int x1, int y1){
    srand(time(NULL)); // Inicializar o gerador de numeros aleatorios
    printf("Gerando minas...\n");
    int i;
    if(debug==1) printf(COLOR_LIGHT_YELLOW"Jogada inicial em      (%d, %d)\n"COLOR_RESET, x1+1, y1+1);

    // Checar se é possivel gerar minas com 0 minas ao redor da primeira jogada
    if(row*col-8 < cont_minas){
        if(debug==1) printf(COLOR_LIGHT_RED"Impossivel gerar minas com 0 minas ao redor da primeira jogada\n"COLOR_RESET);
        if(debug==1) printf(COLOR_LIGHT_YELLOW"Gerando minas...\n"COLOR_RESET);
        // Gerar minas aleatoriamente se nao for possivel gerar com 0 minas ao redor
        for(i=0; i<cont_minas; i++){
            int x = rand() % col;
            int y = rand() % row;
            if(minas[y][x] == 1 || (x == x1 && y == y1)){ // Se ja tiver uma mina, ou se for na primeira jogada, tentar novamente
                if(debug==1) printf(COLOR_LIGHT_RED"Falha ao gerar mina em (%d, %d)\n"COLOR_RESET, x+1, y+1);
                i--;
            }else{
                minas[y][x] = 1; // Se nao tiver, colocar uma mina
                if(debug==1) printf(COLOR_LIGHT_GREEN"Mina colocada em       (%d, %d)\n"COLOR_RESET, x+1, y+1);
            }
        }
    }else{
        if(debug==1) printf(COLOR_LIGHT_GREEN"É possivel gerar minas com 0 minas ao redor da primeira jogada!\n"COLOR_RESET);
        if(debug==1) printf(COLOR_LIGHT_YELLOW"Gerando minas...\n"COLOR_RESET);
        // Não gerar em volta da primeira jogada
        for(i=0; i<cont_minas; i++){
            int x = rand() % col;
            int y = rand() % row;
            if(minas[y][x] == 1){ // Se ja tiver uma mina, ou se for em volta da primeira jogada, tentar novamente
                i--;
            }else if((x == x1 && y == y1) || (x == x1+1 && y == y1) || (x == x1-1 && y == y1) || (x == x1 && y == y1+1) || (x == x1 && y == y1-1) || (x == x1+1 && y == y1+1) || (x == x1-1 && y == y1-1) || (x == x1+1 && y == y1-1) || (x == x1-1 && y == y1+1)){ // Se ja tiver uma mina, ou se for em volta da primeira jogada, tentar novamente
                if(debug==1) printf(COLOR_LIGHT_RED"Falha ao gerar mina em (%d, %d)\n"COLOR_RESET, x+1, y+1);
                i--;
            }else {
                minas[y][x] = 1; // Se nao tiver, colocar uma mina
                if(debug==1) printf(COLOR_LIGHT_GREEN"Mina colocada em       (%d, %d)\n"COLOR_RESET, x+1, y+1);
            }
        }
    }
    if(debug==1) system("pause");
}

void gerar_numeros(){
    int i, j;
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            if(minas[i][j] == 1){
                numeros[i][j] = -1;
            }else{
                // Checar as 8 casas ao redor (se existirem)
                int cont = 0;
                if(i-1>=0 && j-1>=0 && minas[i-1][j-1] == 1){
                    cont++;
                }
                if(i-1>=0 && minas[i-1][j] == 1){
                    cont++;
                }
                if(i-1>=0 && j+1<col && minas[i-1][j+1] == 1){
                    cont++;
                }
                if(j-1>=0 && minas[i][j-1] == 1){
                    cont++;
                }
                if(j+1<col && minas[i][j+1] == 1){
                    cont++;
                }
                if(i+1<row && j-1>=0 && minas[i+1][j-1] == 1){
                    cont++;
                }
                if(i+1<row && minas[i+1][j] == 1){
                    cont++;
                }
                if(i+1<row && j+1<col && minas[i+1][j+1] == 1){
                    cont++;
                }
                numeros[i][j] = cont;
            }
        }
    }
}

void jogar(){
    int x, y;
    char c;
    printf("\nDigite a coordenada (x y): ");
	fflush(stdin);
    scanf("%d %d", &x, &y);
    x -= 1;
    y -= 1;
    // Checar se a coordenada eh valida
    if(x<0 || x>=col || y<0 || y>=row){
        printf("Coordenada invalida, tente novamente.\n");
        jogar();
    }
    if(visivel[y][x] == 0){
        printf("Digite a acao (r - revelar, m - marcar): ");
        fflush(stdin);
        scanf("%c", &c);
        if(c == 'r'){
            visivel[y][x] = 1;
        }else if(c == 'm'){
            visivel[y][x] = 2;
        }else{
            printf("Acao invalida, tente novamente.\n");
            jogar();
        }
    }
    // Checar se a coordenada ja foi revelada
    else if(visivel[y][x] == 1){
        printf("Coordenada ja revelada, tente novamente.\n");
        jogar();
    }
    // Checar se a coordenada ja foi marcada
    else if(visivel[y][x] == 2){
        printf("Digite a acao (r - revelar, d - desmarcar): ");
        fflush(stdin);
        scanf("%c", &c);
        if(c == 'r'){
            visivel[y][x] = 1;
        }else if(c == 'd'){
            visivel[y][x] = 0;
        }else{
            printf("Acao invalida, tente novamente.\n");
            jogar();
        }
    }
    else{
        // Resetar visibilidade (vai que eu fiz alguma merda)
        visivel[y][x] = 0;
        printf("Erro desconhecido, tente novamente.\n");
        jogar();
    }
}

void renderizar(){
    int i, j;
    int coord_x = 1;
    int coord_y = 1;
    system("cls");
    printf("Campo Minado\n");
    printf("    ");

    /* Caracteres especiais 
    218 = ┌
    196 = ─
    191 = ┐
    179 = │
    192 = └
    217 = ┘
    194 = ┬
    195 = ├
    197 = ┼
    180 = ┤
    193 = ┴

    254 = ■
    */

    //Detectar se o número tem mais de um dígito e printar as coordenadas x
    for(i=0; i<col; i++){
        if(i<9){
            printf(" %d ", coord_x);
        }else{
            printf("%d ", coord_x);
        }
        coord_x++;
    }
    
    printf("\n");

    // Printar a 1a linha de divisao ┌─┬─┐
    printf("   ┌"); // ┌
    for(i=0; i<col-1; i++){ // ──┬
        printf("──┬");
    }
    printf("──┐"); // ──┐
    printf("\n");

    // Printar linhas de numeros │  │  │
    for(i=0; i<row; i++){
        // Printar a coordenada y
        if(i<9){
            printf(" %d │", coord_y); // " y │"
        }else{
            printf("%d │", coord_y); // "yy │"
        }
        coord_y++;
        for(j=0; j<col; j++){
            // Se a casa nao foi revelada printar " "
            if(visivel[i][j] == 0){
                printf("  │"); // "  │"
            }
            else if(visivel[i][j] == 1){
                // Se for mina printar M
                if(numeros[i][j]==-1){
                    printf(COLOR_LIGHT_RED" M"COLOR_RESET"│"); // " M│"
                }

                // Senão printar o número
                else{
                    switch (numeros[i][j])
                    {
                    case 1:
                        printf(COLOR_LIGHT_BLUE" %d"COLOR_RESET"│", numeros[i][j]); // " n│"
                        break;
                    
                    case 2:
                        printf(COLOR_GREEN" %d"COLOR_RESET"│", numeros[i][j]); // " n│"
                        break;

                    case 3:
                        printf(COLOR_RED" %d"COLOR_RESET"│", numeros[i][j]); // " n│"
                        break;

                    case 4:
                        printf(COLOR_BLUE" %d"COLOR_RESET"│", numeros[i][j]); // " n│"
                        break;

                    case 5:
                        printf(COLOR_MAGENTA" %d"COLOR_RESET"│", numeros[i][j]); // " n│"
                        break;

                    case 6:
                        printf(COLOR_CYAN" %d"COLOR_RESET"│", numeros[i][j]); // " n│"
                        break;

                    case 7:
                        printf(COLOR_YELLOW" %d"COLOR_RESET"│", numeros[i][j]); // " n│"
                        break;

                    case 8:
                        printf(COLOR_LIGHT_RED" %d"COLOR_RESET"│", numeros[i][j]); // " n│"
                        break;
                    default:
                        printf(" %d│", numeros[i][j]); // " n│"
                        break;
                    }

                    // printf(" %d%c", numeros[i][j], 179); // " n│"
                }
            }
            // Se for bandeira printar "■"
            else if(visivel[i][j] == 2){
                printf(COLOR_LIGHT_YELLOW" ■"COLOR_RESET"│"); // " ■│"
            }
        }
        printf("\n");
        // Printar as linhas de divisao ├──┼──┤
        if(i != row-1){
            printf("   ├");
            for(j=0; j<col-1; j++){
                printf("──┼");
            }
            printf("──┤");
            printf("\n");
        }
    }
    // Printar a ultima linha de divisao └──┴──┘
    printf("   └");
    for(i=0; i<col-1; i++){
        printf("──┴");
    }
    printf("──┘");
}

void atualizar(){
    int i, j;
    // Se o jogador revelou 0, revelar todas as casas adjacentes
    // Repetir até que a matriz visivel nao seja alterada
    int alterou = 1;
    while(alterou){
        alterou = 0;
        for(i=0; i<row; i++){
            for(j=0; j<col; j++){
                if(visivel[i][j] == 1 && numeros[i][j] == 0){
                    //Revelar as casas adjacentes
                    if(i-1 >= 0 && j-1 >= 0 && visivel[i-1][j-1] == 0){
                        visivel[i-1][j-1] = 1;
                        alterou = 1;
                    }
                    if(i-1 >= 0 && visivel[i-1][j] == 0){
                        visivel[i-1][j] = 1;
                        alterou = 1;
                    }
                    if(i-1 >= 0 && j+1 < col && visivel[i-1][j+1] == 0){
                        visivel[i-1][j+1] = 1;
                        alterou = 1;
                    }
                    if(j-1 >= 0 && visivel[i][j-1] == 0){
                        visivel[i][j-1] = 1;
                        alterou = 1;
                    }
                    if(j+1 < col && visivel[i][j+1] == 0){
                        visivel[i][j+1] = 1;
                        alterou = 1;
                    }
                    if(i+1 < row && j-1 >= 0 && visivel[i+1][j-1] == 0){
                        visivel[i+1][j-1] = 1;
                        alterou = 1;
                    }
                    if(i+1 < row && visivel[i+1][j] == 0){
                        visivel[i+1][j] = 1;
                        alterou = 1;
                    }
                    if(i+1 < row && j+1 < col && visivel[i+1][j+1] == 0){
                        visivel[i+1][j+1] = 1;
                        alterou = 1;
                    }
                }
            }
        }
    }

    // Checar se o jogador perdeu
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            if(minas[i][j] == 1 && visivel[i][j] == 1){
                game_over();
            }
        }
    }
    // Checar se o jogador ganhou
    int contv = 0;
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            if(visivel[i][j] == 1){
                contv++;
            }
        }
    }
    if(contv == (row*col)-cont_minas){
        game_win();
    }
}

void game_over(){
    fim = clock(); // Tempo final
    int i, j;
    // Mostrar todas as minas
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            if(minas[i][j] == 1){
                visivel[i][j] = 1;
            }
        }
    }
    renderizar();
    printf("\nVoce perdeu!\n");
    exibir_tempo();
    printf("Jogar novamente? (s/n): ");
    char c;
    fflush(stdin);
    scanf("%c", &c);
    if(c == 's'){
        start();
    }else{
        exit(0);
    }
}

void game_win(){
    fim = clock(); // Tempo final
    int i, j;
    // Mostrar todas as minas
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            if(minas[i][j] == 1){
                visivel[i][j] = 2;
            }
        }
    }
    renderizar();
    printf("\nVoce ganhou!\n");
    exibir_tempo();
    printf("Jogar novamente? (s/n): ");
    char c;
    fflush(stdin);
    scanf("%c", &c);
    if(c == 's'){
        start();
    }else{
        exit(0);
    }
}

void exibir_tempo(){
    tempo = fim - inicio; // Calcula o tempo total em milisegundos
    tempo_seg = tempo / 1000; // Converte para segundos
    tempo_min = tempo_seg / 60; // Converte para minutos
    tempo_hora = tempo_min / 60; // Converte para horas

    tempo_seg -= (tempo_min * 60); // Calcula os segundos restantes
    tempo_min -= (tempo_hora * 60); // Calcula os minutos restantes
    printf("Tempo decorrido: %02dh%02dm%02ds\n", tempo_hora, tempo_min, tempo_seg); // Exibe o tempo decorrido (%02d = 2 digitos)
}