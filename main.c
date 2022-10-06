#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#define ROWMAX 30 // Numero máximo de linhas
#define COLMAX 30 // Numero máximo de colunas

void gerar_minas();
void gerar_numeros();
void jogar();
void renderizar();
void atualizar();
void game_over();
void game_win();
void exibir_tempo();

int row, col; // Tamanho do tabuleiro
int cont_minas; // Quantidade de minas
int visivel[ROWMAX][COLMAX]; // Matriz de visibilidade
int numeros[ROWMAX][COLMAX]; // Matriz de numeros
int minas[ROWMAX][COLMAX]; // Matriz de minas
int inicio, fim, tempo, tempo_seg, tempo_min, tempo_hora; // Variaveis de tempo

int main()
{
    //Definir título 
    SetConsoleTitle("Campo Minado");
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
    system("cls");
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
        row = 30;
        col = 20;
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
    gerar_minas();
    gerar_numeros();
    renderizar();

    inicio = clock(); // Iniciar o cronometro

    while(1){
        jogar();
        atualizar();
        renderizar();
    }
}

void gerar_minas(){
    srand(time(NULL)); // Inicializar o gerador de numeros aleatorios
    int i, j;
    for(i=0; i<cont_minas; i++){
        int x = rand() % row;
        int y = rand() % col;
        if(minas[x][y] == 1){ // Se ja tiver uma mina, tentar novamente
            i--;
        }else{
            minas[x][y] = 1; // Se nao tiver, colocar uma mina
        }
    }
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
    scanf("%d %d", &x, &y);
    x = x-1;
    y = y-1;
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

    /*
        Nas próximas linhas tem varios if col>9,
        isso é para printar o tabuleiro corretamente corretamente caso o número tenha mais de um dígito

        2 - 9        10+
        ┌─┬─┐        ┌──┬──┐
        │x│x│       │xx│xx│
        └─┴─┘        └──┴──┘
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
    printf("   %c", 218); // ┌
    for(i=0; i<col-1; i++){ // ──┬
        printf("%c%c%c", 196, 196, 194);
    }
    printf("%c%c%c", 196, 196, 191); // ──┐
    printf("\n");

    // Printar linhas de numeros │  │  │
    for(i=0; i<row; i++){
        // Printar a coordenada y
        if(i<9){
            printf(" %d %c", coord_y, 179); // yy │
        }else{
            printf("%d %c", coord_y, 179); //  y │
        }
        coord_y++;
        for(j=0; j<col; j++){
            // Se a casa nao foi revelada printar " "
            if(visivel[i][j] == 0){
                printf("  %c", 179); // "  │"
            }
            else if(visivel[i][j] == 1){
                // Se for mina printar M
                if(numeros[i][j]==-1){
                    printf("%c%c%c", 77, 77, 179); // "MM│"
                }

                // Senão printar o número
                else{
                    printf(" %d%c", numeros[i][j], 179); // " n│"
                }
            }
            // Se for bandeira printar "■"
            else if(visivel[i][j] == 2){
                printf("%c%c%c", 254, 254, 179); // "■■│"
            }
        }
        printf("\n");
        // Printar as linhas de divisao ├──┼──┤
        if(i != row-1){
            printf("   %c", 195);
            for(j=0; j<col-1; j++){
                printf("%c%c%c", 196, 196, 197);
            }
            printf("%c%c%c", 196, 196, 180);
            printf("\n");
        }
    }
    // Printar a ultima linha de divisao └──┴──┘
    printf("   %c", 192);
    for(i=0; i<col-1; i++){
        printf("%c%c%c", 196, 196, 193);
    }
    printf("%c%c%c", 196, 196, 217);
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
    int contv = 0, contb = 0, contm = 0;
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            if(visivel[i][j] == 1){
                contv++;
            }
        }
    }
    if(contv == (row*col)-cont_minas /*|| (contm == cont_minas && contb == cont_minas)*/){
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
        main();
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
        main();
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