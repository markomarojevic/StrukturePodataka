#define _CRT_SECURE_NO_WARNINGS	
#include <stdio.h>
#include <string.h>
#define MAX_LINE 1024
#include <stdlib.h>

typedef struct _element* Position;
typedef struct _element {
	//data
	int exp;
	int coeff;
	//link
	Position next;
}element;
int Read_from_file(Position head, FILE* file);
Position create_head();
void FreeList(Position head);
Position create_element(int ekspo, int koef);
int InsertSorted(Position head, Position nE);
Position DeleteAfter(Position target);
int PrintPoliinom(Position first);
int InsertAfter(Position previous, Position to_insert);
Position polinom_addition(Position head_one, Position head_two);
Position polinom_multiplication(Position head_one, Position head_two);
int main() {
	FILE* fp = NULL;


	Position headA = (Position)malloc(sizeof(element));
	if (headA == NULL) {
		printf("Error allocating memory for head!");
		return -1;
	}
	headA->coeff = 0;
	headA->exp = 0;
	headA->next = NULL;

	fp = fopen("polinomi.txt", "r");
	if (fp == NULL) {
		printf("Error opening file!");
		return -1;
	}


	if (Read_from_file(headA, fp) == -1) {
		printf("Error reading from file!\n");
		fclose(fp);
		return -1;
	}


	Position headB = (Position)malloc(sizeof(element));
	if (headB == NULL) {
		printf("Error allocating memory for head!");
		return -1;
	}
	headB->coeff = 0;
	headB->exp = 0;
	headB->next = NULL;
	if (Read_from_file(headB, fp) == -1) {
		printf("Error reading from file!\n");
		fclose(fp);
		return -1;
	}

	fclose(fp);

	printf("Polinom A: \n");
	PrintPoliinom(headA->next);
	printf("Polinom B: \n");
	PrintPoliinom(headB->next);

	Position head_zbroja = polinom_addition(headA, headB);
	printf("Pol A + Pol B : \n");
	PrintPoliinom(head_zbroja->next);

	Position head_umnoska = polinom_multiplication(headA, headB);
	printf("Pol A x Pol B; \n");
	PrintPoliinom(head_umnoska->next);

	FreeList(head_umnoska);
	FreeList(head_zbroja);
	FreeList(headA);
	FreeList(headB);
	return 0;
}
int InsertSorted(Position head, Position nE) {

	Position temp = head;
	if (head == NULL || nE == NULL) {
		printf("head or new element is NULL");
		return -1;
	}


	//Sve dok je eksponent novog clana liste veci od nekog trenutnog shiftamo
	while (temp->next != NULL && temp->next->exp < nE->exp) {
		temp = temp->next;
	}

	if (temp->next == NULL || temp->next->exp != nE->exp) //prazna lista ILI
		//prethodnom while petljom smo se uvjerili da nam je temp sad sigurno VECEG ILI JEDNAKOG eksponenta
		InsertAfter(temp, nE);

	else { //eksponenti jednaki=koef zbrajamo
		int resultCoefficient = nE->coeff + temp->next->coeff;
		if (resultCoefficient == 0) {
			DeleteAfter(temp);
			free(nE);
		}
		else {
			temp->next->coeff = resultCoefficient;
			free(nE);
		}
	}


	return 0;
}
Position DeleteAfter(Position target) {
	Position toDelete = NULL;



	if (target == NULL || target->next == NULL) {
		return NULL;
	}
	toDelete = target->next;
	target->next = toDelete->next;
	free(toDelete);


	return target;
}
int PrintPoliinom(Position first) {
	Position temp = NULL;
	temp = first;
	if (temp == NULL) {
		printf("The list is empty");
		return EXIT_FAILURE;
	}
	while (temp != NULL) {
		printf("%dx^%d ", temp->coeff, temp->exp);
		temp = temp->next;

	}
	printf("\n\n");


	return EXIT_SUCCESS;
}

int Read_from_file(Position head, FILE* file) {
	int numBytes = 0;
	int coeff = 0, expo = 0;
	char buffer[MAX_LINE] = "";
	if (fgets(buffer, MAX_LINE, file) == NULL) {
		printf("There is no more data in the file.");



	}
	Position new_el = NULL;

	printf("Procitan red: %s", buffer);


	char* p_buffer = buffer;
	while (sscanf(p_buffer, "%d %d %n", &coeff, &expo, &numBytes) == 2) {
		printf("%dx^%d ", coeff, expo);
		new_el = create_element(expo, coeff);
		if (new_el == NULL) {
			printf("Error creating element");
			return -1;
		}
		InsertSorted(head, new_el);
		p_buffer += numBytes;
	}
	printf("\n\n");




	return 0;
}
Position create_head() {
	Position head = (Position)malloc(sizeof(element));
	if (head == NULL) {
		printf("Error creating head");
		return NULL;
	}
	head->next = NULL;
	head->coeff = 0;
	head->exp = 0;

	return head;
}
Position polinom_addition(Position head_one, Position head_two) {
	Position new_head = create_head();
	Position i, j;
	i = head_one->next;
	j = head_two->next;
	while (i != NULL || j != NULL) {
		int coeff = 0;
		int exp = 0;
		Position newEl = NULL;
		//1.slucaj=uzimamo element iz polinoma A: AKO JE DRUGI NULL ILI IMA MANJI EKSPONENT
		if (i != NULL && (j == NULL || i->exp > j->exp)) {
			coeff = i->coeff;
			exp = i->exp;
			i = i->next; //shiftamo prvu listu
		}
		//2.Slucaj=uzimamo element iz polinoma B (ovaj iz prvog ima manji eksponent ILI JE NULL)
		else if (j != NULL && (i == NULL || j->exp > i->exp)) {
			coeff = j->coeff;
			exp = j->exp;
			j = j->next;//shiftamo drugu listu

		}
		//3.Slucaj eksponenti su isti=> zbrajamo ih
		else {
			coeff = i->coeff + j->coeff;
			exp = i->exp;
			i = i->next;
			j = j->next; //shiftamo obe liste (iskoristili smo oba clana)
		}

		//sad dodajemo clana s podatcima coeff,exp u trecu listu:
		if (coeff != 0) {
			newEl = create_element(exp, coeff);
			if (newEl == NULL) {
				printf("Error creating element for adding polinom\n");
				return NULL;

			}
			InsertSorted(new_head, newEl);
		}



	}


	return new_head;
}
Position polinom_multiplication(Position head_one, Position head_two) {
	Position new_head = create_head();
	Position i = head_one->next;
	Position j = head_two->next;

	while (i != NULL) {
		Position j_temp = j; //Svaki put cemo postavit temp na prvi clan drugog polinoma jer mnozimo svaki sa svakin
		while (j_temp != NULL) {
			int koef = i->coeff * j_temp->coeff; //koeficijente mnozimo
			int ekspo = i->exp + j_temp->exp;//eksponente zbrajamo
			//stvorimo novi element s dobivenim rez
			Position newEl = create_element(ekspo, koef);
			if (newEl == NULL) {
				printf("Error creating element kod mnozenja polinoma");
				return NULL;
			}
			InsertSorted(new_head, newEl); //umecemo ga u novu listu

			j_temp = j_temp->next; //shiftamo se na drugi element i sad isti ovaj iz prve liste mnozimo s drugim
		}
		i = i->next; //shiftamo prvi pol;opet mnozimo sa svakim 
	}

	return new_head;
}

int InsertAfter(Position previous, Position to_insert) {

	if (previous == NULL || to_insert == NULL) {
		return -1;
	}

	to_insert->next = previous->next;
	previous->next = to_insert;

	return 0;
}
Position create_element(int ekspo, int koef)
{
	Position newEl = NULL;
	newEl = (Position)malloc(sizeof(element));
	if (newEl == NULL) {
		printf("Error allocating memory!");
		return NULL;
	}
	newEl->coeff = koef;
	newEl->exp = ekspo;
	newEl->next = NULL;

	return newEl;
}
void FreeList(Position head)
{
	Position tmp = head;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
}