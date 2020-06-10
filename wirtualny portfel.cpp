#define _CRT_SECURE_NO_WARNINGS
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include "vectors.h"

int bufferMoreData(char* data, size_t size, size_t nmemb, Vector* buffer)
{
	int result = 0;

	if (buffer != NULL)
	{
		result = size * nmemb;
		for (int i = 0; i < strlen(data); i++)
			push_back(buffer, data[i]);
	}
	return result;
}

char* downloadPage(const char* page2Download)
{
	Vector* buffer = constructVector();
	reserve(buffer, 10000);

	CURL* curl;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, page2Download);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, bufferMoreData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

		curl_easy_perform(curl);

		curl_easy_cleanup(curl);
	}

	return buffer->data;
}

float get_price(const char* page2download)
{
	char* page_to_download;

	page_to_download = downloadPage(page2download);

	float act_price = 0;

	for (int i = 0; page_to_download[i] != '\0'; i++)
	{
		if (page_to_download[i] == 'L'
			&& page_to_download[i + 1] == 'a'
			&& page_to_download[i + 2] == 's'
			&& page_to_download[i + 3] == 't')
		{

			int j;
			for (j = i + 6; page_to_download[j] != ','; j++)
			{
				act_price = act_price * 10 + (page_to_download[j] - 48);
			}
			act_price += 0.1 * (page_to_download[j + 1] - 48) + 0.01 * (page_to_download[j + 2] - 48);
			break;

		}
	}
	return act_price;
}

float get_percent(const char* page2download)
{
	char* downloaded_page = downloadPage(page2download);

	float change = 0;

	for (int i = 0; downloaded_page[i] != '\0'; i++)
	{
		if (downloaded_page[i] == 's'
			&& downloaded_page[i + 1] == '='
			&& downloaded_page[i + 2] == '"'
			&& downloaded_page[i + 3] == 'v')
		{
			int j;
			int flag = 0;
			for (j = i + 10; downloaded_page[j] != ','; j++)
			{
				if (downloaded_page[j] == '-')
				{
					flag = 1;
				}
				else
					change = change * 10 + (downloaded_page[j] - 48);
			}
			change += 0.1 * (downloaded_page[j + 1] - 48) + 0.01 * (downloaded_page[j + 2] - 48);
			if (flag == 1)
				change *= -1;
			break;

		}
	}
	return change;
}

void make_file(const char* name, const char* name_of_file)
{
	char plik[40];
	strcpy(plik, name);
	strcat(plik, "/");
	strcat(plik, name_of_file);
	strcat(plik, ".txt");

	FILE* file;
	fopen_s(&file, plik, "w");
	fprintf(file, "%d", 0);
	fclose(file);
}

void make_user(const char* name)
{
	char command[40] = { 'm', 'k', 'd', 'i', 'r', ' ' };
	int i;
	for (i = 0; i < strlen(name); i++)
		command[i + 6] = name[i];

	system(command);

	make_file(name, name);
	make_file(name, "gotowka");
	make_file(name, "CDPROJEKT");
	make_file(name, "PKNORLEN");
	make_file(name, "CCC");
	make_file(name, "PEKAO");
	make_file(name, "PZU");
	

}

float choose_company(const char* company)
{
	if (company == "CDPROJEKT.txt")
		return get_price("https://www.bankier.pl/inwestowanie/profile/quote.html?symbol=CDPROJEKT");
	if (company == "PKNORLEN.txt")
		return get_price("https://www.bankier.pl/inwestowanie/profile/quote.html?symbol=PKNORLEN");
	if (company == "CCC.txt")
		return get_price("https://www.bankier.pl/inwestowanie/profile/quote.html?symbol=CCC");
	if (company == "PEKAO.txt")
		return get_price("https://www.bankier.pl/inwestowanie/profile/quote.html?symbol=PEKAO");
	if (company == "PZU.txt")
		return get_price("https://www.bankier.pl/inwestowanie/profile/quote.html?symbol=PZU");
}

float choose_company_for_percent(const char* company)
{
	if (company == "CDPROJEKT.txt")
		return get_percent("https://www.bankier.pl/inwestowanie/profile/quote.html?symbol=CDPROJEKT");
	if (company == "PKNORLEN.txt")
		return get_percent("https://www.bankier.pl/inwestowanie/profile/quote.html?symbol=PKNORLEN");
	if (company == "CCC.txt")
		return get_percent("https://www.bankier.pl/inwestowanie/profile/quote.html?symbol=CCC");
	if (company == "PEKAO.txt")
		return get_percent("https://www.bankier.pl/inwestowanie/profile/quote.html?symbol=PEKAO");
	if (company == "PZU.txt")
		return get_percent("https://www.bankier.pl/inwestowanie/profile/quote.html?symbol=PZU");
}

void deposit(const char* name, int how_much)
{
	char plik[40];
	strcpy(plik, name);
	strcat(plik, "/");
	strcat(plik, "gotowka.txt");

	FILE* fp;
	int actual;
	fopen_s(&fp, plik, "r");
	fscanf_s(fp, "%d", &actual);
	fclose(fp);

	fopen_s(&fp, plik, "w+");
	fprintf(fp, "%d", how_much + actual);
	fclose(fp);

	strcpy(plik, name);
	strcat(plik, "/");
	strcat(plik, name);
	strcat(plik, ".txt");

	fopen_s(&fp, plik, "r");
	fscanf_s(fp, "%d", &actual);
	fclose(fp);

	fopen_s(&fp, plik, "w+");
	fprintf(fp, "%d", how_much + actual);
	fclose(fp);
}

void buy(const char* name, const char* company, int how_many)
{

	float cost = choose_company(company);
	cost *= how_many;

	char plik[40];
	strcpy(plik, name);
	strcat(plik, "/");
	strcat(plik, "gotowka.txt");
	FILE* cash;
	fopen_s(&cash, plik, "r");

	float actual_cash;
	fscanf_s(cash, "%f", &actual_cash);
	fclose(cash);

	if (actual_cash >= cost)
	{
		strcpy(plik, name);
		strcat(plik, "/");
		strcat(plik, company);
		FILE* Company;
		fopen_s(&Company, plik, "r");

		int number_of_shares;
		fscanf_s(Company, "%d", &number_of_shares);
		fclose(Company);

		if (number_of_shares + how_many < 0)
		{
			printf("Nie posiadasz tylu akcji\n");
			return;
		}

		strcpy(plik, name);
		strcat(plik, "/");
		strcat(plik, "gotowka.txt");

		FILE* cash;
		fopen_s(&cash, plik, "w");
		fprintf(cash, "%f", actual_cash - cost);
		fclose(cash);

		strcpy(plik, name);
		strcat(plik, "/");
		strcat(plik, company);

		FILE* fp;
		fopen_s(&fp, plik, "w");
		fprintf(fp, "%d", number_of_shares + how_many);
		fclose(fp);
	}
	else
		printf("Brak srodkow na koncie\n");
	
}

int number_of_shares(const char* name, const char* company)
{
	char plik[40];
	strcpy(plik, name);
	strcat(plik, "/");
	strcat(plik, company);
	FILE* Company;
	fopen_s(&Company, plik, "r");

	int number_of_shares;
	fscanf_s(Company, "%d", &number_of_shares);
	fclose(Company);

	return number_of_shares;
}

float wallet(const  char* name, const char* file)
{
	char plik[40];
	strcpy(plik, name);
	strcat(plik, "/");
	strcat(plik, file);
	strcat(plik, ".txt");
	FILE* fp;
	fopen_s(&fp, plik, "r");

	float how_much;
	fscanf_s(fp, "%f", &how_much);
	fclose(fp);

	return how_much;
}

void guidebook(const char* name)
{
	printf("Witamy w wirtualnym portfelu %s!, zapoznaj sie z ponizszymi instrukcjami, aby z latwoscia poruszac sie w aplikacji.\n", name);
	printf("\n");
	printf("1. Kiedy jestes proszony o wybranie jakiejs opcji, zawsze wpisuj liczby calkowite wystepujace przed wybierana opcja.\n");
	printf("2. Zalecane jest zaktualizowanie zawartosci portfela przed wyjsciem z aplikacj.\n");
	printf("3. Poszczegolne funkcje w panelu glownym pomagaja w poruszaniu sie po nim.\n");
	system("pause");
}

void login(char* name)
{
	printf("Witamy w wirtualnym portfelu, wybierz jedna z opcji:\n");
	int what_to_do;
	printf("1. Zaloguj sie\n");
	printf("2. Stworz nowe konto\n");
	scanf("%d", &what_to_do);

	printf("Podaj nazwe uzytkownika: ");
	scanf("%s", name);

	if (what_to_do == 2)
	{
		guidebook(name);
		make_user(name);
	}

	system("cls");

}

void panel(char* name)
{
	int what_to_do = 0;
	while (what_to_do != 8)
	{
		printf("1. Wplac pieniadze\n");
		printf("2. Wyplac pieniadze\n");
		printf("3. Kup akcje\n");
		printf("4. Sprzedaj akcje\n");
		printf("5. Przegladnij kursy akcji\n");
		printf("6. Zaktualizuj zawartosc portfela\n");
		printf("7. Pokaz zawartosc portfela\n");
		printf("8. Wyjdz\n");

		scanf("%d", &what_to_do);
		int how_much;
		int which_company;
		int how_many;
		system("cls");

		if (what_to_do > 8)
		{
			printf("Zle wybrana opcja\n");
			continue;
		}

		int CDPnumb=0, PKNnumb=0, CCCnumb=0, PEKnumb=0, PZUnumb=0;
		float CDP, PKN, CCC, PEK, PZU;
		float money = 0;

		switch (what_to_do)
		{
		case 1:
			printf("Ile chcesz wplacic: ");
			scanf("%d", &how_much);
			deposit(name, how_much);
			break;
		case 2:
			printf("Ile chcesz wyplacic: ");
			scanf("%d", &how_much);
			deposit(name, -1*how_much);
			break;
		case 3:
			CDP = choose_company("CDPROJEKT.txt");
			PKN = choose_company("PKNORLEN.txt");
			CCC = choose_company("CCC.txt");
			PEK = choose_company("PEKAO.txt");
			PZU = choose_company("PZU.txt");

			printf("Wybierz firme: \n");
			printf("1. CDPROJEKT %.2f zl\n", CDP);
			printf("2. PKNORLEN %.2f zl\n", PKN);
			printf("3. CCC %.2f zl\n", CCC);
			printf("4. PEKAO %.2f zl\n", PEK);
			printf("5. PZU %.2f zl\n", PZU);
			printf("6. Wyjdz\n");

			scanf("%d", &which_company);

			if (which_company == 6)
				break;

			printf("Ile akcji chcesz kupic: \n");
			scanf("%d", &how_many);

			if (which_company == 1)
				buy(name, "CDPROJEKT.txt", how_many);
			if (which_company == 2)
				buy(name, "PKNORLEN.txt", how_many);
			if (which_company == 3)
				buy(name, "CCC.txt", how_many);
			if (which_company == 4)
				buy(name, "PEKAO.txt", how_many);
			if (which_company == 5)
				buy(name, "PZU.txt", how_many);
			break;
		case 4:
			printf("Wybierz firme: \n");
			printf("1. CDPROJEKT, posiadasz %d akcji\n", number_of_shares(name, "CDPROJEKT.txt"));
			printf("2. PKNORLEN, posiadasz %d akcji\n", number_of_shares(name, "PKNORLEN.txt"));
			printf("3. CCC, posiadasz %d akcji\n", number_of_shares(name, "CCC.txt"));
			printf("4. PEKAO, posiadasz %d akcji\n", number_of_shares(name, "PEKAO.txt"));
			printf("5. PZU, posiadasz %d akcji\n", number_of_shares(name, "PZU.txt"));
			printf("6. Wyjdz\n");

			scanf("%d", &which_company);

			if (which_company == 6)
				break;

			printf("Ile akcji chcesz sprzedac: \n");
			scanf("%d", &how_many);

			if (which_company == 1)
				buy(name, "CDPROJEKT.txt", -1 * how_many);
			if (which_company == 2)
				buy(name, "PKNORLEN.txt", -1 * how_many);
			if (which_company == 3)
				buy(name, "CCC.txt", -1 * how_many);
			if (which_company == 4)
				buy(name, "PEKAO.txt", -1 * how_many);
			if (which_company == 5)
				buy(name, "PZU.txt", -1 * how_many);
			break;
		case 5:
			CDP = choose_company("CDPROJEKT.txt");
			PKN = choose_company("PKNORLEN.txt");
			CCC = choose_company("CCC.txt");
			PEK = choose_company("PEKAO.txt");
			PZU = choose_company("PZU.txt");

			float CDPperc, PKNperc, CCCperc, PEKperc, PZUperc;
			CDPperc = choose_company_for_percent("CDPROJEKT.txt");
			PKNperc = choose_company_for_percent("PKNORLEN.txt");
			CCCperc = choose_company_for_percent("CCC.txt");
			PEKperc = choose_company_for_percent("PEKAO.txt");
			PZUperc = choose_company_for_percent("PZU.txt");

			printf("CDPROJEKT: %.2f zl, zmiana w procentach %.2f \n", CDP, CDPperc);
			printf("PKNORLEN: %.2f zl, zmiana w procentach  %.2f \n", PKN, PKNperc);
			printf("CCC: %.2f zl, zmiana w procentach  %.2f \n", CCC, CCCperc);
			printf("PEKAO: %.2f zl, zmiana w procentach  %.2f \n", PEK, PEKperc);
			printf("PZU: %.2f zl, zmiana w procentach  %.2f \n", PZU, PZUperc);
			printf("\n");
			break;
		case 6:
			CDP = choose_company("CDPROJEKT.txt");
			PKN = choose_company("PKNORLEN.txt");
			CCC = choose_company("CCC.txt");
			PEK = choose_company("PEKAO.txt");
			PZU = choose_company("PZU.txt");

			CDPnumb = number_of_shares(name, "CDPROJEKT.txt");
			PKNnumb = number_of_shares(name, "PKNORLEN.txt");
			CCCnumb = number_of_shares(name, "CCC.txt");
			PEKnumb = number_of_shares(name, "PEKAO.txt");
			PZUnumb = number_of_shares(name, "PZU.txt");

			float cash;
			char plik[40];
			strcpy(plik, name);
			strcat(plik, "/");
			strcat(plik, "gotowka.txt");
			FILE* fp;
			fopen_s(&fp, plik, "r");
			fscanf(fp, "%f", &cash);
			fclose(fp);
	
			money = CDP * CDPnumb + PKN * PKNnumb + CCC * CCCnumb + PEK * PEKnumb + PZU * PZUnumb;

			strcpy(plik, name);
			strcat(plik, "/");
			strcat(plik, name);
			strcat(plik, ".txt");
			fopen_s(&fp, plik, "w");
			fprintf(fp, "%f", money + cash);
			fclose(fp);
			
			printf("Aktualna wartosc portfela to: %.2f zl\n", money + cash);
			break;
		case 7:
			printf("Ogolna wartosc konta: %.2f zl\n", wallet(name, name));
			printf("W tym gotowka: %.2f zl\n", wallet(name, "gotowka"));

			printf("1. CDPROJEKT, posiadasz %d akcji\n", number_of_shares(name, "CDPROJEKT.txt"));
			printf("2. PKNORLEN, posiadasz %d akcji\n", number_of_shares(name, "PKNORLEN.txt"));
			printf("3. CCC, posiadasz %d akcji\n", number_of_shares(name, "CCC.txt"));
			printf("4. PEKAO, posiadasz %d akcji\n", number_of_shares(name, "PEKAO.txt"));
			printf("5. PZU, posiadasz %d akcji\n\n", number_of_shares(name, "PZU.txt"));
			break;
		}

	}

}

int main()
{
	char* name = (char*)malloc(20 * sizeof(char));

	login(name);

	panel(name);

	free(name);

	return 0;
}
