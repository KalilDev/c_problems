#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef NDEBUG
#define ensure(expr)\
if (!(expr)) {\
    exit(EXIT_FAILURE);\
}
#else
#define ensure(expr) assert(expr);
#endif

void print_board(char *board, int side) {
    for (int c = 0; c < side + 2; c++) {
        putchar('_');
    }
    putchar('\n');
    for (int r = 0; r < side; r++) {
        putchar('|');
        for (int c = 0; c < side; c++) {
            printf(c == 0 ? "%c" : " %c", board[r*side+c]);
        }
        puts("|");
    }
    for (int c = 0; c < side + 2; c++) {
        fputs("𛲆", stdout);
    }
    putchar('\n');
}

const char bomba = 'b';
const char vazio = 'o';
const char xis = 'x';

int main()
{
	while (true) {
		// Rodar o código para um jogo.
	    // Passos:
        // ler numero de partidas
        int num_partidas;
        puts("Digite quantas vezes vc quer jogar campo minado");
        ensure(scanf("%d", &num_partidas) == 1);
		// ler o tamanho do tabuleiro quadrado
		int side;
        puts("Digite o lado do campo minado que vc quer");
		ensure(scanf("%d", &side) == 1);
		// criar o tabuleiro
		char tabuleiro[side][side];
		// ler o tabuleiro do usuario
        printf("Digite o campo minado, com %c sendo espaços vazios e %c sendo bombas\n", xis, bomba);
       	for (int i = 0; i < side; i++) {
         	for (int j = 0; j < side; j++) {
                char *cell = &tabuleiro[i][j];
               	ensure(scanf(" %c", cell) == 1);
                ensure(*cell == bomba || *cell == xis);
            }
        }
        print_board((char *)tabuleiro, side);
		// Passos: Para cada partida
		for (int partida = 0; partida < num_partidas; partida++) {
			// Contar quantas bombas tem no tabuleiro
        	int n_bombas = 0;
        	for (int i = 0; i < side; i++) {
            	for (int j = 0; j < side; j++) {
                	if (tabuleiro[i][j] == bomba) {
                    	n_bombas++;
                	}
            	}
        	}
			// Ler quantas jogadas o jogador quer fazer
			int n_jogadas;
            puts("Digite quantas jogadas você quer fazer");
			ensure(scanf("%d", &n_jogadas) == 1);
            ensure(n_jogadas > 0);
			// Para cada jogada
			bool lost = false;
			for (int jogada = 0; jogada < n_jogadas; jogada++) {
				// Ler a coordenada
				int i, j;
                puts("Digite uma coordenada x e y para a proxima jogada");
				ensure(scanf("%d %d", &i, &j) == 2);
				// Caso vc tenha perdido, não fazer mais nada só ler sua entrada
				if (lost) {
					continue;
				}
                printf("Coordenada lida tem i: %d, j: %d\n", i, j);
				int na_coordenada = tabuleiro[i][j];
				// Caso tenha uma bomba, voce perdeu, mas vamos continuar
				// iterando pra ler suas entradas.
				if (na_coordenada == bomba) {
					lost = true;
					continue;
				}
				// Ignorar jogadas em pontos vazios
				if (na_coordenada == vazio) {
					continue;
				}
				// Caso n tenha bomba vizinha
				bool tem_bomba_vizinha = false;
				// Para cada posição relativa de célula vizinha de i, j
                for (int p = -1; p <= 1; p++) {
					// Garantir que i+p está em [0..side)
					if ((i == 0 && p < 0) || (i == side - 1 && p > 0)) {
						continue;
					}
                	for (int q = -1; q <= 1; q++) {
						// Garantir que j+q está em [0..side)
						if ((j == 0 && q < 0) || (i == side - 1 && q > 0)) {
							continue;
						}
						// Ignorar posicao relativa 0, 0
						if (p == 0 && q == 0) {
							continue;
						}
                        tem_bomba_vizinha = tem_bomba_vizinha || (tabuleiro[i + p][j + q] == bomba);
                    }
				}
				// Caso n tenha bomba vizinha, o 
				if (tem_bomba_vizinha) {
					tabuleiro[i][j] = vazio;
					continue;
				}
				// Para cada posição relativa de célula vizinha de i, j
                for (int p = -1; p <= 1; p++) {
					// Caso o i seja 0, i + p vai estar fora do limite permitido
					// no tabuleiro [0..side) quando p for negativo
					if (i == 0 && p < 0) {
						continue;
					}
					
					// Caso o i seja side-1, i + p vai estar fora do limite permitido
					// no tabuleiro [0..side) quando p for positivo
					if (i == side - 1 && p > 0) {
						continue;
					}
                	for (int q = -1; q <= 1; q++) {
						// Caso o j seja 0, j + q vai estar fora do limite permitido
						// no tabuleiro [0..side) quando q for negativo
						if (j == 0 && q < 0) {
							continue;
						}
					
						// Caso o j seja side-1, j + q vai estar fora do limite permitido
						// no tabuleiro [0..side) quando q for positivo
						if (i == side - 1 && q > 0) {
							continue;
						}
						
						// Caso q e p sejam 0, a célula é a célula que o
						// usuario digitou, logo ela n é uma vizinha.
						if (p == 0 && q == 0) {
							continue;
						}
						
						// Setar este vizinho para vazio
                        tabuleiro[i + p][j + q] = vazio;
                    }
               	}
			}
            print_board((char *)tabuleiro, side);
			if (lost) {
            	puts("PERDEU");
				continue;
			}
			// Checar se faltou terminar	
            bool faltou_terminar = false;
            for (int i = 0; i < side; i++) {
                for (int j = 0; j < side; j++) {
                    if (tabuleiro[i][j] == xis) {
                        faltou_terminar = true;
                    }
                }
            }
			if (faltou_terminar) {
                puts("FALTOU TERMINAR");
                puts("O campo minado no final era:");
                print_board((char*)tabuleiro, side);
				continue;
			}
            puts("GANHOU");
		}
		// Ler se o usuario quer jogar de novo ou não
		puts("Você quer começar um novo jogo? (s/S)im ou (n/N)ão)");
		// initialize to false so that EOF doesnt retry.
		bool will_retry = false;
		for (char should_retry = getchar(); should_retry != EOF; should_retry = getchar()) {
			if (isspace(should_retry)) {
                continue;
            }
            if (should_retry == 's' || should_retry == 'S') {
				will_retry = true;
				break;
			}
			if (should_retry == 'n' || should_retry == 'N') {
				break;
			}
			printf("Não entendi '%c', só entendo s, S, n e N.\n", should_retry);
		}
		if (!will_retry) {
			break;
		}
	}
}
