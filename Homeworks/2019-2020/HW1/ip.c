//ANICA ALEXANDRU GABRIEL *fost* 313CB
//PC - TEMA 1

#include <stdio.h>

/*
 * Citire de adresa ce contine "."
 */
static void read_ip_dot(unsigned int *part1, unsigned int *part2,
		                unsigned int *part3, unsigned int *part4)
{
	scanf("%u.%u.%u.%u", part1, part2, part3, part4);
}

/*
 * Formeaza un unsigned int din cele 4 numere separate de puncte
 */
static unsigned int formare_int(unsigned int part1, unsigned int part2,
            	        		unsigned int part3, unsigned int part4)
{
	return (part1 << 24) + (part2 << 16) + (part3 << 8) + part4;
}

/*
 * Transformma o masca de tipul /NR_INTREG intr-un numar intreg
 * format zecimal pentru adresa 
 */
static unsigned int masca_int(int mask)
{
	unsigned int result;
	int i;

	for (i = 31 - mask; i >= 0; i--)
		result = result | (1 << i);

	result = ~result;
	return result;
}

/*
 * Citire de adresa format zecimal cu masca
 */
static void read_adress_mask(unsigned int *part1, unsigned int *part2,
                             unsigned int *part3, unsigned int *part4,
                             unsigned int *mask)
{
	scanf("%u.%u.%u.%u/%d", part1, part2, part3, part4, mask);
}

/*
 * Afiseaza adresa format zecimal
 */
static void afisare_punct(unsigned int address)
{
	printf("%u.%u.%u.%u", address >> 24, (address >> 16) & 255,
		   (address >> 8) & 255, address & 255);
}

/*
 * Afisare pe biti a unei adrese
 */
static void afisare_bit(unsigned int adress)
{
	int i;

	for (i = 31; i >= 0; i--)
	{
		if ((i % 8 == 0) && i)
			printf("%d.", ((adress >> i) & 1));
		else
			printf("%d", ((adress >> i) & 1));
	}
} 

static void TASK0(unsigned int IP_1, unsigned int MSK_2)
{
	printf("-0 ");
	afisare_punct(IP_1);
	printf("/%d\n", MSK_2);
}

static void TASK1(unsigned int MSK_2p)
{
	printf("-1 ");
	afisare_punct(MSK_2p);
	printf("\n");
}

static void TASK2(unsigned int aux_adress)
{
	printf("-2 ");
	printf("%o.%o.%o.%o ", aux_adress >> 24, (aux_adress >> 16) & 255,
		   (aux_adress >> 8) & 255, aux_adress & 255);
	printf("%X.%X.%X.%X\n", aux_adress >> 24, (aux_adress >> 16) & 255,
		   (aux_adress >> 8) & 255, aux_adress & 255);
}

static void TASK3(unsigned int IP_1, unsigned int MSK_2p)
{
	printf("-3 ");
	afisare_punct(IP_1 & MSK_2p);
	printf("\n");
}

static void TASK4(unsigned int IP_1, unsigned int MSK_2p)
{
	printf("-4 ");
	afisare_punct(IP_1 | (~(MSK_2p)));
	printf("\n");
}

void TASK5(unsigned int IP_1, unsigned int IP_2, unsigned int MSK_2p)
{
	if ((IP_1 & MSK_2p) == (IP_2 & MSK_2p))
		printf("-5 DA\n");
	else
		printf("-5 NU\n");
}

/*
 * Explicatii in readme
 */
int TASK6(unsigned int adress)
{	
    int i;
	unsigned int k = (adress >> 31) & 1;

	for (i = 31; i >= 0; i--)
	{
		if (((adress >> i) & 1) > k)
			return 0;
		else
			k = (adress >> i) & 1;
	}

	return 1;
}

/*
 * Functie pentru TASK 7, se foloseste de structura TASK6
 */
static unsigned int corectare(unsigned int adress)
{
	int i, valid = 1;
	unsigned int k = (adress >> 31) & 1;

	for (i = 31; i >= 0; i--)
	{
		if (((adress >> i) & 1) > k)
		{
			valid = 0;
			break;
		}
		else
		{
			k = (adress >> i) & 1;
		}
	}

	if (valid == 0)
	{
		int j;
		for (j = i; j >= 0; j --)
			adress = adress & (~(1 << j));
	}

	return adress;
}

static void TASK7(unsigned int MSK_1)
{
	printf("-7 ");

	if (!TASK6(MSK_1))
	{
		MSK_1 = corectare(MSK_1);
	}

	afisare_punct(MSK_1);
		
    printf("\n");
}

void TASK8(unsigned int IP_1)
{
	printf("-8 ");
	afisare_bit(IP_1);
	printf("\n");
}

static void TASK9(unsigned int IP_2)
{
	int N, i;
	unsigned int address, mask, part1, part2, part3, part4;

	printf("-9 ");
	scanf("%d", &N);

	for (i = 0; i < N; i++)
	{
		read_adress_mask(&part1, &part2, &part3, &part4, &mask);
		address = formare_int(part1, part2, part3, part4);
		mask = masca_int(mask);
		if ((IP_2 & mask) == (address & mask))
			printf("%d ", i);
	}

	printf("\n");
}
 

int main(void)
{
	unsigned int MSK_1, MSK_2, MSK_2p = 0, IP_1, IP_2;
	unsigned int part1, part2, part3, part4;
	int nr_tests, i;

	scanf("%d", &nr_tests);

	for (i = 1; i <= nr_tests; i++)
	{
		printf("%d\n", i);

		/* MSK_1-citire si formare de octeti */
		read_ip_dot(&part1, &part2, &part3, &part4); 
		MSK_1 = formare_int(part1, part2, part3, part4);

		/* MSK 2 citire si formare de octeti */
		scanf("%d", &MSK_2);
		MSK_2p = masca_int( MSK_2);

		/* IP_1 & IP_2 (citire si formare) */
		read_ip_dot(&part1, &part2, &part3, &part4);
		IP_1 = formare_int(part1, part2, part3, part4);

		read_ip_dot(&part1, &part2, &part3, &part4);
		IP_2 = formare_int(part1, part2, part3, part4);

		TASK0(IP_1, MSK_2);
		TASK1(MSK_2p);
		TASK2(MSK_1);
		TASK3(IP_1, MSK_2p);
		TASK4(IP_1, MSK_2p);
		TASK5(IP_1, IP_2, MSK_2p);

		if (TASK6(MSK_1))
			printf("-6 DA\n");
		else
			printf("-6 NU\n");

		TASK7(MSK_1);
		TASK8(IP_1);
		TASK9(IP_2);
	}

	return 0;
}
