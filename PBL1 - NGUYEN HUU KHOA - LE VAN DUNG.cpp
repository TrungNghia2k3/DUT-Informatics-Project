#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include "string.h"
#define name_limit 20
#define mssv_limit 11
#define glimit 5
#define adrlimit 30
#define email_limit 30
#define year_limit 3
#define fal_limit 4
#define filename_limit 50
#define temp1 5
#define alias_limit 20
#define MAX 100
struct date
{
	int day;
	int month;
	int year;
};
typedef struct date date;
struct lop
{
	// int soluonghocsinh;
	char namhoc[year_limit];
	char faculty[fal_limit];
};
typedef struct lop classINFO;
struct sinhvien
{

	char MSSV[mssv_limit] = "";
	char firstname[name_limit] = "";
	char lastname[name_limit] = "";
	char gender[glimit] = "";
	char address[adrlimit] = "";
	date birth;
	char email[email_limit] = "";
	int stt;
	int erased_toggle = 0;
	int toggle_done_MSSV = 0;
	int toggle_done_email = 0;
	int toggle1 = 0; // kiem tra xem danh sach da sap xep hay cap ma sv chua
	int toggle2 = 0; // kiem tra xem da cap ma sv chua
};
typedef sinhvien SV;

/* Function */
void xoaXuongDong(char x[]);
void xoadash(char x[]);
void structswap(SV *sv1, SV *sv2);
void docfile(SV sv[], int *p, classINFO *lop, int *sign);
void capnhatsinhvien(SV sv[], int *position);
void themsinhvien(SV &sv, int i);
void sapxepsinhvien(SV sv[], int n);
void capmasv(SV sv[], int n, classINFO lop);
void capemailsv(SV sv[], int n);
void in(SV sv);
void indanhsach(SV sv[], int n);
void xuatfile(SV sv[], int n);
void timkiemsinhvien(SV sv[], int n);
void xoasinhvien(SV sv[], int &n);
void title();
void menu();
void option(int *loop2, SV sv[], int *position, classINFO *lop);
void end(int *loop1);

int main()
{
	int loop1 = 1;
	while (loop1 > 0)
	{
		int position = 0;
		SV sv[MAX];
		classINFO lop;
		int FILEsign = 1;
		int loop2 = 1; // flag

		docfile(sv, &position, &lop, &FILEsign); // doc file

		if (!(FILEsign == 0))
			while (loop2 > 0)
			{
				title();
				menu();
				option(&loop2, sv, &position, &lop);
			}
		else
			printf("KHONG THE MO FILE HOAC TEN FILE KHONG HOP LE");
		end(&loop1);
	}
}

void xoaXuongDong(char x[])
{
	size_t len = strlen(x);
	if (x[len - 1] == '\n')
		x[len - 1] = ' ';
}

void xoadash(char x[])
{
	size_t len = strlen(x);
	for (int i = 0; i <= len - 1; i++)
	{
		if (x[i] == '_')
			x[i] = ' ';
	}
}

void structswap(SV *sv1, SV *sv2)
{
	SV temp = *sv1;
	*sv1 = *sv2;
	*sv2 = temp;
}

void docfile(SV sv[], int *p, classINFO *lop, int *sign)
{
	char classFILE[filename_limit];
	printf("\t%c DO AN LAP TRINH TINH TOAN %c\n", 16, 17);
	printf("%c THUC HIEN BOI: NGUYEN HUU KHOA & LE VAN DUNG %c\n\n", 16, 17);
	printf("Nhap ten file lop can mo (luu y: nho them phan mo rong (vd: .txt, .bin,...) sau ten file): ");
	fgets(classFILE, sizeof(classFILE), stdin);
	xoaXuongDong(classFILE);
	FILE *f;
	int addnumb;
	char nian[year_limit];
	char fac[fal_limit];
	f = fopen(classFILE, "r");
	if (f == NULL)
	{

		*sign = 0;
	}
	else
	{

		fscanf(f, "%d", &addnumb);		// doc so luong hoc sinh
		fscanf(f, "%s", &lop->namhoc);	// doc nam hoc
		fscanf(f, "%s", &lop->faculty); // doc ma nganh

		for (int i = 1; i <= addnumb; i++)
		{
			sv[i].stt = i;
			fscanf(f, "%s", &sv[i].firstname);	 // doc ho lot
			fscanf(f, "%s", &sv[i].lastname);	 // doc ten
			fscanf(f, "%d", &sv[i].birth.day);	 // doc ngay sinh
			fscanf(f, "%d", &sv[i].birth.month); // doc thang sinh
			fscanf(f, "%d", &sv[i].birth.year);	 // doc nam sinh
			fscanf(f, "%s", &sv[i].gender);		 // doc gioi tinh
			fscanf(f, "%s", &sv[i].address);	 // doc dia chi
		}

		for (int i = 1; i <= addnumb; i++)
		{
			xoadash(sv[i].firstname); // xu li chuoi ho lot
			xoadash(sv[i].address);	  // xu li chuoi dia chi
		}

		*p = addnumb; // tang gia tri cua p (position) len tuong ung voi so luong hoc sinh co san trong file
	}

	fclose(f);
	system("cls");
}

void capnhatsinhvien(SV sv[], int *position)
{
	int n;
	// fflush(stdin);
	while (n > MAX - *position)
	{ // kiem tra so luong sinh vien them vao co dat gioi han?
		printf("\nNHAP SO LUONG SINH VIEN CAN THEM: ");
		scanf("%d", &n);
	}

	for (int i = *position + 1; i <= *position + n; i++)
	{
		themsinhvien(sv[i], i);
		sv[i].stt = i;
	}

	*position += n;
}

void themsinhvien(SV &sv, int i)
{
	fflush(stdin);
	char a[glimit] = "Nam"; // chuoi cho gender la Nam
	char b[glimit] = "Nu";	// chuoi cho gender la Nu
	char select_char;
	int t = 1;
	printf("Nhap thong tin cho sv thu %.4d", i);
	fflush(stdin);
	printf("\nNhap ho lot: ");
	fgets(sv.firstname, sizeof(sv.firstname), stdin);
	xoaXuongDong(sv.firstname);
	printf("\nNhap ten: ");
	fflush(stdin);
	scanf("%s", &sv.lastname);
	xoaXuongDong(sv.lastname);
	do
	{
		printf("\nNhap ngay thang nam sinh (nhap lien tiep 3 muc ngay, thang va nam sinh): ");
		fflush(stdin);
		scanf("%d%d%d", &sv.birth.day, &sv.birth.month, &sv.birth.year);
	} while (sv.birth.day < 1 || sv.birth.day > 31 || sv.birth.month > 12 || sv.birth.month < 1);
	fflush(stdin);
	do
	{
		printf("\nGioi tinh: ");
		printf("\n1.NAM");
		printf("\n2.NU");
		printf("\nNHAP LUA CHON CUA BAN: ");
		scanf("%c", &select_char);
		fflush(stdin);
		if (select_char == '1')
			strcat(sv.gender, a), t = 0;
		else if (select_char == '2')
			strcat(sv.gender, b), t = 0;
		else
			;
	} while (t > 0);

	printf("\nDia chi: ");
	fflush(stdin);
	fgets(sv.address, sizeof(sv.address), stdin);
	xoaXuongDong(sv.address);
}

void sapxepsinhvien(SV sv[], int n)
{								 // bubble
	for (int i = 1; i <= n; i++) // sap xep ho lot
		for (int j = i + 1; j <= n; j++)
			if (strcmp(sv[i].firstname, sv[j].firstname) > 0)
				structswap(&sv[i], &sv[j]);

	for (int i = 1; i <= n; i++) // sap xep ten
		for (int j = i + 1; j <= n; j++)
			if (strcmp(sv[i].lastname, sv[j].lastname) > 0)
				structswap(&sv[i], &sv[j]);

	for (int i = 1; i <= n; i++)
		sv[i].toggle1 = 1; // sau khi sap xep thi =1
}

void capmasv(SV sv[], int n, classINFO lop)
{
	char sott[temp1];
	for (int i = 1; i <= n; i++)
		if (sv[i].toggle1 == 0)
		{ // check neu da sap xep roi
			printf("\nCHUA SAP XEP DANH SACH SINH VIEN, KHONG THE CAP MA!");
			return;
		}
	for (int i = 1; i <= n; i++)
	{
		if (sv[i].toggle_done_MSSV == 0)
		{ // neu cap roi thi ko cap nua
			strcat(sv[i].MSSV, lop.faculty);
			strcat(sv[i].MSSV, lop.namhoc);

			for (int j = 1; j <= n; j++)
			{
				if (i == j)
					;
				else if (sv[i].stt == sv[j].stt)
				{
					sv[i].stt++;
				}
			}

			sprintf(sott, "%.4d", sv[i].stt);
			strcat(sv[i].MSSV, sott);
			sv[i].toggle2 = 1;
			sv[i].toggle_done_MSSV = 1;
		}
	}
	return;
}

void capemailsv(SV sv[], int n)
{
	char alias[alias_limit] = "@sv.dut.udn.vn"; // hau to cho email sv
	for (int i = 1; i <= n; i++)
		if (sv[i].toggle2 == 0)
		{ // kiem tra neu da cap ma sinh vua hay chua?
			printf("\nCHUA CAP MA SO SINH VIEN, KHONG THE CAP EMAIL!");
			return;
		}
	for (int i = 1; i <= n; i++)
	{ // tien hanh tu dong tao va cap email sinh vien
		if (sv[i].toggle_done_email == 0)
		{
			strcat(sv[i].email, sv[i].MSSV);
			strcat(sv[i].email, alias);
			sv[i].toggle_done_email = 1; // danh dau la da cap email sinh vien
		}
	}
}

void in(SV sv)
{
	printf("\n%-10s \t%-20s  %10.2d/%.2d/%.2d  %-10s %-20s %-10s %-4s", sv.firstname, sv.lastname, sv.birth.day, sv.birth.month, sv.birth.year, sv.gender, sv.address, sv.MSSV, sv.email);
}

void indanhsach(SV sv[], int n)
{
	for (int i = 1; i <= n; i++)
	{
		in(sv[i]);
	}
}

void xuatfile(SV sv[], int n)
{
	char tenfile[filename_limit];
	char temp[temp1] = ".txt";
	printf("\nSave as name: ");
	fgets(tenfile, sizeof(tenfile), stdin);
	xoaXuongDong(tenfile);
	strcat(tenfile, temp);
	FILE *f;
	f = fopen(tenfile, "w");
	if (f == NULL)
	{
		printf("DINH DANG FILE KHONG HOP LE");
	}
	else
	{
		for (int i = 1; i <= n; i++)
		{
			fprintf(f, "%-10s \t%-20s  %10.2d/%.2d/%.2d  %-10s %-20s %-10s %-4s\n", sv[i].firstname, sv[i].lastname, sv[i].birth.day, sv[i].birth.month, sv[i].birth.year, sv[i].gender, sv[i].address, sv[i].MSSV, sv[i].email);
		}
	}
	fclose(f);
}

void timkiemsinhvien(SV sv[], int n)
{
	char fID[mssv_limit];
	char fName[name_limit];
	char c;
	int temp, index;
	printf("\n1.TIM KIEM THEO MA SINH VIEN");
	printf("\n2.TIM KIEM THEO TEN");
	printf("\nNHAP LUA CHON CUA BAN: ");
	scanf("%c", &c);
	fflush(stdin);
	switch (c)
	{
	case '1':
	{
		printf("\nnhap masv can tim: ");
		scanf("%s", fID);
		xoaXuongDong(fID);
		fflush(stdin);
		printf("danh sach cac sv co ma sv %s :", fID);
		for (int i = 1; i <= n; i++)
		{
			if (strcmp(sv[i].MSSV, fID) == 0)
			{
				temp = 1;  // neu co thi =1
				index = i; // vi tri phan tu dang tim kiem
			}
		}
		if (temp)
			in(sv[index]);
		else
			printf("\nko co sinh vien nao co ma sinh vien la %s", fID);
		break;
	}

	case '2':
	{
		printf("\nnhap ten can tim: ");
		scanf("%s", fName);
		xoaXuongDong(fName);
		fflush(stdin);
		printf("danh sach cac sv co ten: %s", fName);
		for (int i = 1; i <= n; i++)
			if (strcmp(sv[i].lastname, fName) == 0)
			{
				temp = 1;
				in(sv[i]);
			}
		if (temp == 0)
			printf("\nko co sinh vien nao co ten la %s", fName);
	}
	}
}

void xoasinhvien(SV sv[], int &n)
{
	char fID[mssv_limit];
	int i = 1;
	int temp = 0;
	printf("\nNHAP MA SINH VIEN CUA SINH VIEN CAN XOA: ");
	scanf("%s", fID);
	xoaXuongDong(fID);
	fflush(stdin);
	while (i <= n)
	{
		if (strcmp(sv[i].MSSV, fID) == 0)
		{
			temp = 1;
			for (int j = i; j <= n; j++)
			{
				sv[j] = sv[j + 1];
			}
			n--;
		}
		i++;
	}
	if (temp == 0)
		printf("\nKHONG CO SINH VIEN NAO CO MA SINH VIEN LA %s DE XOA", fID);
}

void title()
{
	printf("\n\t%c DO AN LAP TRINH TINH TOAN %c\n", 16, 17);
	printf("%c THUC HIEN BOI: NGUYEN HUU KHOA & LE VAN DUNG %c\n", 16, 17);
}

void menu()
{
	printf("\n%c------QUAN LY DANH SACH SINH VIEN------%c\n", 16, 17);
	printf("%c 1.THEM SINH VIEN\n", 16);
	printf("%c 2.TIM KIEM SINH VIEN\n", 16);
	printf("%c 3.XOA SINH VIEN\n", 16);
	printf("%c 4.XUAT DANH SACH SINH VIEN RA MAN HINH\n", 16);
	printf("%c 5.XUAT DANH SACH SINH VIEN RA FILE\n", 16);
	printf("%c 6.SAP XEP DANH SACH SINH VIEN\n", 16);
	printf("%c 7.CAP MA SO SINH VIEN\n", 16);
	printf("%c 8.CAP EMAIL SINH VIEN\n", 16);
	printf("%c 0.KET THUC THAO TAC VOI FILE NAY\n", 16);
}

void option(int *loop2, SV sv[], int *position, classINFO *lop)
{
	char select_var;
	printf("\nNHAP LUA CHON CUA BAN: ");
	scanf("%c", &select_var);
	fflush(stdin);
	switch (select_var)
	{
	case '1':
	{
		capnhatsinhvien(sv, position);
		printf("\nNHAN PHIM BAT KI DE TIEP TUC");
		getch();
		break;
	}
	case '6':
	{
		sapxepsinhvien(sv, *position);
		printf("\nNHAN PHIM BAT KI DE TIEP TUC");
		getch();
		break;
	}
	case '7':
	{
		capmasv(sv, *position, *lop);
		printf("\nNHAN PHIM BAT KI DE TIEP TUC");
		getch();
		break;
	}
	case '8':
	{
		capemailsv(sv, *position);
		printf("\nNHAN PHIM BAT KI DE TIEP TUC");
		getch();
		break;
	}
	case '2':
	{
		timkiemsinhvien(sv, *position);
		printf("\nNHAN PHIM BAT KI DE TIEP TUC");
		getch();
		break;
	}
	case '3':
	{
		xoasinhvien(sv, *position);
		printf("\nNHAN PHIM BAT KI DE TIEP TUC");
		getch();
		break;
	}
	case '4':
	{
		indanhsach(sv, *position);
		printf("\nNHAN PHIM BAT KI DE TIEP TUC");
		getch();
		break;
	}
	case '5':
	{
		xuatfile(sv, *position);
		printf("\nNHAN PHIM BAT KI DE TIEP TUC");
		getch();
		break;
	}
	case '0':
	{
		*loop2 = 0;
		break;
	}
	case '\n':
	{
		break;
	}
	}
	system("cls");
}

void end(int *loop1)
{
	printf("\n NHAP 1 DE TIEP TUC THAO TAC VOI FILE KHAC\n HOAC 0 DE KET THUC CHUONG TRINH ");
	scanf("%d", loop1);
	fflush(stdin);
}
