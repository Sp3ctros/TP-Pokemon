#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LEN 100
#define Max_num_pokemons 100
#define Bonus_ataque 1.2f
#define Debuff_ataque 0.8f

#define Nome_arquivo "text.txt"

// Cores 
#define RESET "\033[0m"
#define FOGO_COR "\033[31m"
#define VERDE_COR "\033[32m"
#define ELETRICO_COR "\033[33m"
#define AGUA_COR "\033[34m"
#define ROXO "\033[35m"
#define GELO_COR "\033[36m"
#define BRANCO_COR "\033[37m"
#define PEDRA_COR "\033[090m"
#define BALD "\033[1m"

// tipos de pokemons como constantes para o calculo de batalha
enum TipoPokemon { ELETRICO = 1, AGUA = 2, FOGO = 3, GELO = 4, PEDRA = 5};



// Estrutura de um Pokémon
struct pokemon
{
    char nome[MAX_STRING_LEN];
    char tipo[MAX_STRING_LEN];
    int ataque;
    int defesa;
    float vida;
    char cor[MAX_STRING_LEN];
};

// EStrutura de um treinador
typedef struct
{
    struct pokemon cartel_pokemon[Max_num_pokemons];
    int num_pokemons;
    int num_pokemons_vivos;
} treinador;

//escolhe a cor do pokemon baseado no tipo
void escolher_cor(char* cor, const char* tipo) 
{
    switch (tolower(tipo[0]))
    {
        case 'a':
            strcpy(cor, AGUA_COR);
            break;
        case 'e':
            strcpy(cor, ELETRICO_COR);
            break;
        case 'f':
            strcpy(cor, FOGO_COR);
            break;
        case 'g':
            strcpy(cor, GELO_COR);
            break;
        case 'p':
            strcpy(cor, PEDRA_COR);
            break;
        default:
            strcpy(cor, RESET);
            break;
    }
}


// Função para ler pokémons de um arquivo e separar no vetor treinador A e B
int lerPokemons(FILE *arq, treinador *treinadorA, treinador *treinadorB)
{
    char linha_buffer[MAX_STRING_LEN];
    fscanf(arq, "%d %d\n", &treinadorA->num_pokemons, &treinadorB->num_pokemons); // Pega o numero de pokemons de cada treinador
    if (treinadorA->num_pokemons < 1 || treinadorB->num_pokemons < 1)
    {
        printf("Num pokemons:\nTreinador A: %d\nTreinador B: %d\n", treinadorA->num_pokemons, treinadorB->num_pokemons);
        return 1;
    }
    else if (treinadorA->num_pokemons > 100 || treinadorB->num_pokemons > 100)
    {
        printf("Num pokemons:\nTreinador A: %d\nTreinador B: %d\n", treinadorA->num_pokemons, treinadorB->num_pokemons);
        return 2;
    }
    treinadorA->num_pokemons_vivos = treinadorA->num_pokemons;
    treinadorB->num_pokemons_vivos = treinadorB->num_pokemons;
    for (int i = 0; i < treinadorA->num_pokemons + treinadorB->num_pokemons; i++) // separa os dados de cada pokemon
    {
        fgets(linha_buffer, MAX_STRING_LEN, arq);
        if (i < treinadorA->num_pokemons)
        {
            //separa os dados do pokemon e verifica se ele tem 5 caracteristicas.
            if (sscanf(linha_buffer, "%s %d %d %f %s", treinadorA->cartel_pokemon[i].nome, &treinadorA->cartel_pokemon[i].ataque, &treinadorA->cartel_pokemon[i].defesa, &treinadorA->cartel_pokemon[i].vida, treinadorA->cartel_pokemon[i].tipo) != 5)
            {
                printf("%s", linha_buffer);
                return 3;
            }
            escolher_cor(treinadorA->cartel_pokemon[i].cor, treinadorA->cartel_pokemon[i].tipo);
        }
        else
        {
            if (sscanf(linha_buffer, "%s %d %d %f %s", treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].nome, &treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].ataque, &treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].defesa, &treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].vida, treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].tipo) != 5)
            {
                printf("%s", linha_buffer);
                return 3;
            }
            escolher_cor(treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].cor, treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].tipo);

        }
    }
    return 0;
}



void mostrar_dados(treinador *treinadorA, treinador *treinadorB)
{
    printf(BALD"------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("------------------------------------------------------------INFORMACOES DOS TREINADORES---------------------------------------------------------\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%40s %80s\n", "Treinador A","Treinador B");
    printf("%40s:%4d %80s:%4d\n", "Numeros de pokemons", treinadorA->num_pokemons, "Numeros de pokemons",treinadorB->num_pokemons);
    printf("%-20s %-6s %-6s %-6s %-6s %52s %-20s %-6s %-6s %-6s %-6s\n",
           "Nome", "Ataque", "Defesa", "Vida", "Tipo","", "Nome", "Ataque", "Defesa", "Vida", "Tipo" RESET);

    int max_pokemons = (treinadorA->num_pokemons > treinadorB->num_pokemons) ? treinadorA->num_pokemons  : treinadorB->num_pokemons;

    for (int i = 0; i < max_pokemons; i++)
    {
        if (i < treinadorA->num_pokemons)
        {
            printf("%s%-20s %-6d %-6d %-6.0f %-20s"RESET,
                   treinadorA->cartel_pokemon[i].cor,
                   treinadorA->cartel_pokemon[i].nome,
                   treinadorA->cartel_pokemon[i].ataque,
                   treinadorA->cartel_pokemon[i].defesa,
                   treinadorA->cartel_pokemon[i].vida,
                   treinadorA->cartel_pokemon[i].tipo);
        }
        else
        {
            printf("%-20s %-6s %-6s %-6s", "", "", "", "");
        }

        printf("%40s", "");

        if (i < treinadorB->num_pokemons)
        {
            printf("%s%-20s %-6d %-6d %-6.0f %-20s"RESET,
                   treinadorB->cartel_pokemon[i].cor,
                   treinadorB->cartel_pokemon[i].nome,
                   treinadorB->cartel_pokemon[i].ataque,
                   treinadorB->cartel_pokemon[i].defesa,
                   treinadorB->cartel_pokemon[i].vida,
                   treinadorA->cartel_pokemon[i].tipo);
        }

        printf("\n");
    }
    printf(BALD"-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("--------------------------------------------------------------INFORMACOES DA BATALHA-----------------------------------------------------------------\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n"RESET);

}

// converter o tipo de pokemon para valor numérico
int converterTipo(char*tipo)
{
    switch(tolower(tipo[0]))
    {
        case 'a':
            return AGUA;
        case 'e':
            return ELETRICO;
        case 'f':
            return FOGO;
        case 'g':
            return GELO;
        case 'p':
            return PEDRA;
        default:
            return 0; // Erro! Pokemon nao pertence a nenhum tipo pré estabelecido.
    }
}

// Função recursiva de batalha entre treinadores
int pokebatalha(treinador *atacante, int index_A, treinador *defensor, int index_B)
{   int tipo_atacante = converterTipo(atacante->cartel_pokemon[index_A].tipo);
    int tipo_defensor = converterTipo(defensor->cartel_pokemon[index_B].tipo);
    if (tipo_atacante == 0 || tipo_defensor == 0)
    {
        printf("\033[H\033[J");
        printf("Erro! Pokemon nao pertence a nenhum tipo pre estabelecido. [FOGO, AGUA, GELO, ELETRICO, PEDRA]\n");
        return 1; //erro no tipo
    }
    int power_relation = converterTipo(atacante->cartel_pokemon[index_A].tipo) - converterTipo(defensor->cartel_pokemon[index_B].tipo);
    float ataque_atacante = atacante->cartel_pokemon[index_A].ataque;
    float defesa_defensor = defensor->cartel_pokemon[index_B].defesa;
    switch (power_relation)
    {
    case -1:
        ataque_atacante *= Bonus_ataque;
        break;
    case 1:
        ataque_atacante *= Debuff_ataque;
        break;
    case 4:
        ataque_atacante *= Bonus_ataque;
        break;
    case -4:
        ataque_atacante *= Debuff_ataque;
    default:
        break;
    }
    if (ataque_atacante > defesa_defensor)
    {
        defensor->cartel_pokemon[index_B].vida -= (ataque_atacante - defesa_defensor);
    }
    else
    {
        defensor->cartel_pokemon[index_B].vida--;
    }

    if (defensor->cartel_pokemon[index_B].vida <= 0)
    {
        defensor->num_pokemons_vivos--;
        printf("%s" "%s" RESET VERDE_COR " venceu " RESET"%s" "%s\n" RESET, atacante->cartel_pokemon[index_A].cor, atacante->cartel_pokemon[index_A].nome, defensor->cartel_pokemon[index_B].cor, defensor->cartel_pokemon[index_B].nome);
        if (defensor->num_pokemons_vivos <= 0)
        {
            return 0;
        }
        index_B++;

    }
    if (pokebatalha(defensor, index_B, atacante, index_A) == 1)
    {
        return 1;
    }
    return 0;
}




// Mostra todos pokemons que nao foram derrotados
void mostrar_sobreviventes(treinador *treinadorA, treinador *treinadorB)
{
    printf(BALD"Pokemons sobreviventes: \n"RESET);
    for(int i = 0; i < treinadorA->num_pokemons + treinadorB->num_pokemons; i++)
    {
        if (i < treinadorA->num_pokemons)
        {
            if (treinadorA->cartel_pokemon[i].vida > 0)
             {
                printf("%s%s\n"RESET, treinadorA->cartel_pokemon[i].cor,treinadorA->cartel_pokemon[i].nome);

             }
        }
        else
        {
            if (treinadorB->cartel_pokemon[i-treinadorA->num_pokemons].vida > 0)
            {
                printf("%s%s\n"RESET, treinadorB->cartel_pokemon[i-treinadorA->num_pokemons].cor, treinadorB->cartel_pokemon[i-treinadorA->num_pokemons].nome);
            }
        }
    }
}

// Mostra todos pokemons que foram derrotados
void mostrar_derrotados(treinador *treinadorA, treinador *treinadorB)
{
    printf(BALD"Pokemons derrotados: \n"RESET);
    for(int i = 0; i < treinadorA->num_pokemons + treinadorB->num_pokemons; i++)
    {
        if (i < treinadorA->num_pokemons)
        {
            if (treinadorA->cartel_pokemon[i].vida <= 0)
             {
                printf("%s%s\n"RESET, treinadorA->cartel_pokemon[i].cor,treinadorA->cartel_pokemon[i].nome);
             }
        }
        else
        {
            if (treinadorB->cartel_pokemon[i-treinadorA->num_pokemons].vida <= 0)
            {
                printf("%s%s\n"RESET, treinadorB->cartel_pokemon[i-treinadorA->num_pokemons].cor, treinadorB->cartel_pokemon[i-treinadorA->num_pokemons].nome);
            }
        }
    }
}




int main()
{
    treinador treinadorA, treinadorB;
    FILE *arq;
    arq = fopen(Nome_arquivo, "r");
    if (arq == NULL)
    {
        printf("Erro ao abrir arquivo!\n");
        return 1;
    }
    //separa os dados dos pokemons e dos treinadores e verifica os cenarios de erro
    switch (lerPokemons(arq, &treinadorA, &treinadorB))
    {
    case 1:
        printf("Erro! Os Treinadores precisam ter no minimo 1 pokemon!\n");
        return 2;
    case 2:
        printf("Erro! Os Treinadores precisam ter no maximo 100 pokemons!\n");
        return 3;
    case 3:
        printf("Erro! Os pokemons precisam ter 5 caracteristicas (Nome, ataque, defesa, vida e tipo)\n");
        return 4;
    default:
        break;

    }
    int pok_A_index = 0, pok_B_index = 0;
    //BATALHAAAA!
    mostrar_dados(&treinadorA, &treinadorB);
    if (pokebatalha(&treinadorA, pok_A_index, &treinadorB, pok_B_index) == 1) return 4;
    printf(BALD"-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("--------------------------------------------------------------------RESULTADO------------------------------------------------------------------------\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf(treinadorA.num_pokemons_vivos == 0 ? ROXO"Jogador 2 venceu\n"RESET: ROXO"Jogador 1 venceu\n"RESET);
    mostrar_sobreviventes(&treinadorA, &treinadorB);
    mostrar_derrotados(&treinadorA, &treinadorB);
    printf(BALD"-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n"RESET);
    return 0;
}
