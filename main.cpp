#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_STUDENTS 100

/* Mảng song song lưu thông tin sinh viên */
char studentID[MAX_STUDENTS][11];	 // ID sinh viên (tự động tăng)
char studentCode[MAX_STUDENTS][9];	 // Mã số sinh viên
char studentEmail[MAX_STUDENTS][30]; // Email sinh viên
char firstName[MAX_STUDENTS][20];	 // Họ lót
char lastName[MAX_STUDENTS][20];	 // Tên
char gender[MAX_STUDENTS][5];		 // Giới tính
char address[MAX_STUDENTS][30];		 // Địa chỉ
char email[MAX_STUDENTS][30];		 // Email
int birthDay[MAX_STUDENTS];			 // Ngày sinh
int birthMonth[MAX_STUDENTS];		 // Tháng sinh
int birthYear[MAX_STUDENTS];		 // Năm sinh

/* Các thông tin chung của lớp */
int totalStudents = 0; // Số lượng sinh viên hiện tại
int academicYear;	   // Niên khóa
int majorCode;		   // Mã ngành

/* Biến trạng thái */
bool sorted = 0;			   // Đánh dấu danh sách đã sắp xếp chưa
bool generatedStudentCode = 0; // Đánh dấu đã cấp mã SV chưa
bool generatedEmail = 0;	   // Đánh dấu đã cấp email chưa

/* Khai báo các hàm chính */
void addStudent();
void sortStudents();
void deleteStudent(int index);
void searchStudent();
void generateStudentCode();
void generateEmail();
void printStudents();
void readFile(const char *fileName);
void createFile();
void menu();

/* Validation */
int isAlphaString(const char *str);
int isLeapYear(int year);
int isValidDate(int day, int month, int year);
int getValidStudentCount();
int getValidStudentIndex();

/* Utilities */
void capitalizeWords(char *str);
void swapStrings(char *a, char *b);
void swapInts(int *a, int *b);
void toLowerCase(char *str);
bool containsSubstring(char *str, char *sub);

/* Start main */
int main()
{
	readFile("data.txt"); // Đọc dữ liệu từ file
	menu();
	return 0;
}
/* End main */

/* Start add student */
void addStudent()
{
	// Sinh ID tự động
	sprintf(studentID[totalStudents], "%03d", totalStudents + 1);

	// Nhập họ lót
	while (1)
	{
		printf("👉 Nhập họ lót: ");
		fgets(firstName[totalStudents], sizeof(firstName[totalStudents]), stdin);
		firstName[totalStudents][strcspn(firstName[totalStudents], "\n")] = '\0'; // Xóa ký tự xuống dòng

		if (strlen(firstName[totalStudents]) > 0 && isAlphaString(firstName[totalStudents]))
		{
			capitalizeWords(firstName[totalStudents]); // Viết hoa chữ cái đầu mỗi từ
			break;
		}
		printf("❌ Họ lót không hợp lệ! Chỉ được nhập chữ cái. Vui lòng nhập lại.\n");
	}

	// Nhập tên
	while (1)
	{
		printf("👉 Nhập tên: ");
		fgets(lastName[totalStudents], sizeof(lastName[totalStudents]), stdin);
		lastName[totalStudents][strcspn(lastName[totalStudents], "\n")] = '\0';

		if (strlen(lastName[totalStudents]) > 0 && isAlphaString(lastName[totalStudents]))
		{
			capitalizeWords(lastName[totalStudents]); // Viết hoa chữ cái đầu
			break;
		}
		printf("❌ Tên không hợp lệ! Chỉ được nhập chữ cái. Vui lòng nhập lại.\n");
	}

	// Nhập ngày sinh
	while (1)
	{
		printf("📅 Nhập ngày sinh (DD/MM/YYYY): ");
		if (scanf("%d %d %d", &birthDay[totalStudents], &birthMonth[totalStudents], &birthYear[totalStudents]) == 3)
		{
			if (isValidDate(birthDay[totalStudents], birthMonth[totalStudents], birthYear[totalStudents]))
				break;
		}
		printf("❌ Ngày sinh không hợp lệ! Vui lòng nhập lại.\n");
		while (getchar() != '\n')
			; // Xóa bộ nhớ đệm
	}

	// Nhập giới tính
	while (1)
	{
		printf("👉 Chọn giới tính (0 - Nu, 1 - Nam): ");
		int choice;
		if (scanf("%d", &choice) == 1 && (choice == 0 || choice == 1))
		{
			strcpy(gender[totalStudents], (choice == 0) ? "Nu" : "Nam");
			while (getchar() != '\n')
				; // Xóa bộ nhớ đệm
			break;
		}
		printf("❌ Giới tính không hợp lệ! Chỉ được nhập 0 hoặc 1. Vui lòng nhập lại.\n");
		while (getchar() != '\n')
			;
	}

	// Nhập địa chỉ
	while (1)
	{
		printf("👉 Nhập địa chỉ: ");
		fgets(address[totalStudents], sizeof(address[totalStudents]), stdin);
		address[totalStudents][strcspn(address[totalStudents], "\n")] = '\0';

		if (strlen(address[totalStudents]) > 0)
		{
			capitalizeWords(address[totalStudents]); // Viết hoa chữ cái đầu mỗi từ
			break;
		}
		printf("❌ Địa chỉ không được để trống! Vui lòng nhập lại.\n");
	}

	totalStudents++;
	sorted = 0;
	generatedStudentCode = 0;
	generatedEmail = 0;

	printf("✅ Đã thêm sinh viên thành công!\n");
}
/* End add student */

/* Start sort students (bubble sort tối ưu) */
void sortStudents()
{
	if (totalStudents <= 1)
		return; // Không cần sắp xếp nếu danh sách trống hoặc có 1 sinh viên

	sorted = 1;	  // Giả định danh sách đã sắp xếp
	bool swapped; // Kiểm tra xem có hoán đổi nào không

	for (int i = 0; i < totalStudents - 1; i++)
	{
		swapped = false; // Mặc định không có hoán đổi

		for (int j = 0; j < totalStudents - i - 1; j++)
		{
			// So sánh theo lastName trước, nếu trùng thì so sánh firstName
			if (strcmp(lastName[j], lastName[j + 1]) > 0 ||
				(strcmp(lastName[j], lastName[j + 1]) == 0 && strcmp(firstName[j], firstName[j + 1]) > 0))
			{
				// Hoán đổi thông tin sinh viên
				swapStrings(firstName[j], firstName[j + 1]);
				swapStrings(lastName[j], lastName[j + 1]);
				swapStrings(gender[j], gender[j + 1]);
				swapStrings(address[j], address[j + 1]);

				swapInts(&birthDay[j], &birthDay[j + 1]);
				swapInts(&birthMonth[j], &birthMonth[j + 1]);
				swapInts(&birthYear[j], &birthYear[j + 1]);

				swapped = true; // Đánh dấu có hoán đổi
			}
		}

		// Nếu không có hoán đổi nào, dừng vòng lặp
		if (!swapped)
			break;
	}

	sorted = 1; // Khi vòng lặp kết thúc, danh sách đã sắp xếp
}
/* End sort students */

/* Start delete student */
void deleteStudent(int index)
{
	if (totalStudents == 0)
	{
		printf("❌ Không có sinh viên để xóa!\n");
		return;
	}

	printf("✅ Đã xóa sinh viên có STT: %03d (%s %s)\n", index + 1, firstName[index], lastName[index]);

	// Dời các phần tử sau index lên trên
	for (int i = index; i < totalStudents - 1; i++)
	{
		strcpy(firstName[i], firstName[i + 1]);
		strcpy(lastName[i], lastName[i + 1]);
		strcpy(gender[i], gender[i + 1]);
		strcpy(address[i], address[i + 1]);
		birthDay[i] = birthDay[i + 1];
		birthMonth[i] = birthMonth[i + 1];
		birthYear[i] = birthYear[i + 1];
	}

	totalStudents--; // Giảm tổng số sinh viên
}
/* End delete student */

/* Start search student */
void searchStudent()
{
	char input[20];
	printf("\n🔍 Nhập từ khóa tìm kiếm (Tên%s): ", generatedStudentCode ? " hoặc MSSV" : "");
	scanf("%s", input);

	int found = 0;
	// In tiêu đề bảng
	printf("\n🔎 Kết quả tìm kiếm:\n");
	printf("%-5s %-18s %-11s %-12s %-15s %-20s", "ID", "Họ lót", "Tên", "Ngày sinh", "Giới tính", "Địa chỉ");
	if (generatedStudentCode)
		printf(" %-12s", "MSSV"); // Thêm MSSV nếu đã tạo
	if (generatedEmail)
		printf(" %-25s", "Email"); // Thêm Email nếu đã tạo
	printf("\n");

	printf("---------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < MAX_STUDENTS; i++)
	{
		if (containsSubstring(lastName[i], input) || (generatedStudentCode && containsSubstring(studentCode[i], input)))
		{
			printf("%-5s %-15s %-10s %02d/%02d/%02d  %-12s %-15s",
				   studentID[i], firstName[i], lastName[i],
				   birthDay[i], birthMonth[i], birthYear[i],
				   gender[i], address[i]);

			if (generatedStudentCode)
				printf(" %-12s", studentCode[i]); // In MSSV nếu đã tạo
			if (generatedEmail)
				printf(" %-25s", studentEmail[i]); // In Email nếu đã tạo

			printf("\n");
			found = 1;
		}
	}

	if (!found)
	{
		printf("❌ Không tìm thấy sinh viên!\n");
	}
}
/* End search student */

/* Start generate student code */
void generateStudentCode()
{
	for (int i = 0; i < totalStudents; i++)
	{
		sprintf(studentCode[i], "%03d%02d%03d", majorCode, academicYear, i + 1);
	}

	generatedStudentCode = 1;
	printf("✅ Đã tạo mã sinh viên thành công!\n");
};
/* End generate student code */

/* Start generate email */
void generateEmail()
{
	for (int i = 0; i < totalStudents; i++)
	{
		sprintf(studentEmail[i], "%s@sv.dut.udn.vn", studentCode[i]);
	}

	generatedEmail = 1;
	printf("✅ Đã tạo email sinh viên thành công!\n");
};
/* End generate email */

/* Start print students */
void printStudents()
{
	printf("🔥 Số lượng sinh viên: %d\n", totalStudents);
	printf("🔥 Niên khóa: %d\n", academicYear);
	printf("🔥 Mã ngành: %d\n", majorCode);

	// In tiêu đề bảng
	printf("%-5s %-18s %-11s %-12s %-15s %-20s", "ID", "Họ lót", "Tên", "Ngày sinh", "Giới tính", "Địa chỉ");
	if (generatedStudentCode)
		printf(" %-12s", "MSSV"); // Thêm MSSV nếu đã tạo
	if (generatedEmail)
		printf(" %-25s", "Email"); // Thêm Email nếu đã tạo
	printf("\n");

	printf("--------------------------------------------------------------------------------------------------------------------\n");

	// In thông tin sinh viên
	for (int i = 0; i < totalStudents; i++)
	{
		printf("%-5s %-15s %-10s %02d/%02d/%02d  %-12s %-15s",
			   studentID[i], firstName[i], lastName[i],
			   birthDay[i], birthMonth[i], birthYear[i],
			   gender[i], address[i]);

		if (generatedStudentCode)
			printf(" %-12s", studentCode[i]); // In MSSV nếu đã tạo
		if (generatedEmail)
			printf(" %-25s", studentEmail[i]); // In Email nếu đã tạo

		printf("\n");
	}
}
/* End print students */

/* Start read file */
void readFile(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("❌ Không thể mở file %s!\n", filename);
		return;
	}

	// Đọc dòng đầu tiên: số lượng sinh viên, niên khóa, mã ngành
	if (fscanf(file, "%d,%d,%d\n", &totalStudents, &academicYear, &majorCode) != 3)
	{
		printf("❌ Lỗi khi đọc số lượng sinh viên, niên khóa, mã ngành!\n");
		fclose(file);
		return;
	}

	// Kiểm tra số lượng sinh viên hợp lệ
	if (totalStudents > MAX_STUDENTS)
	{
		printf("❌ Lỗi: Số lượng sinh viên vượt quá giới hạn (%d)!\n", MAX_STUDENTS);
		fclose(file);
		return;
	}

	// Đọc danh sách sinh viên
	for (int i = 0; i < totalStudents; i++)
	{
		if (fscanf(file, "%4[^,],%19[^,],%19[^,],%d,%d,%d,%4[^,],%29[^\n]\n",
				   studentID[i], firstName[i], lastName[i],
				   &birthDay[i], &birthMonth[i], &birthYear[i],
				   gender[i], address[i]) != 8)
		{
			printf("❌ Lỗi khi đọc dữ liệu sinh viên %d!\n", i + 1);
			break;
		}
	}

	fclose(file);
}
/* End read file */

/* Start create file */
void createFile()
{
	char filename[50];
	sprintf(filename, "%d%d.txt", academicYear, majorCode); // Tạo tên file theo format academicYear+majorCode.txt

	FILE *file = fopen(filename, "w"); // Mở file ở chế độ ghi

	if (file == NULL)
	{
		printf("❌ Không thể mở file để ghi!\n");
		return;
	}

	// Ghi tiêu đề
	fprintf(file, "MSSV,Họ lót,Tên,Ngày,Tháng,Năm,Giới tính,Địa chỉ,Email\n");

	// Ghi dữ liệu sinh viên
	for (int i = 0; i < totalStudents; i++)
	{
		fprintf(file, "%s,%s,%s,%02d,%02d,%04d,%s,%s,%s\n",
				studentCode[i], firstName[i], lastName[i],
				birthDay[i], birthMonth[i], birthYear[i],
				gender[i], address[i], studentEmail[i]);
	}

	fclose(file);
	printf("✅ Lưu file thành công: %s\n", filename);
}
/* End create file */

/* Start menu */
void menu()
{
	int choice, index;
	do
	{
		printf("\n📌 MENU:\n");
		printf("✨ 1. Thêm sinh viên\n");
		printf("✨ 2. Sắp xếp danh sách\n");
		printf("✨ 3. Xóa sinh viên\n");
		printf("✨ 4. Tìm sinh viên\n");
		printf("✨ 5. Cấp Mã sinh viên\n");
		printf("✨ 6. Cấp email\n");
		printf("✨ 7. In danh sách\n");
		printf("✨ 8. Thoát\n");
		printf("➡️  Chọn: ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
		{
			int n = getValidStudentCount(); // Gọi hàm kiểm tra
			for (int i = 0; i < n; i++)
			{
				printf("\n👉 Nhập thông tin sinh viên %d:\n", i + 1);
				addStudent();
			}
			break;
		}
		case 2:
			if (!sorted) // Nếu danh sách chưa sắp xếp
			{
				sortStudents();
				printf("✅ Danh sách sinh viên đã được sắp xếp\n");
			}
			else
				printf("✅ Danh sách sinh viên đã được sắp xếp\n");
			break;
		case 3:
		{
			int index = getValidStudentIndex(); // Gọi hàm nhập STT
			deleteStudent(index);
		}
		break;
		case 4:
			searchStudent();
			break;
		case 5:
			if (sorted)
				generateStudentCode();
			else
				printf("❌ Danh sách chưa được sắp xếp. Vui lòng sắp xếp trước!\n");
			break;
		case 6:

			if (generatedStudentCode)
				generateEmail();
			else
				printf("❌ Chưa cấp mã sinh viên. Không thể tạo email!\n");
			break;
		case 7:
			printStudents();
			break;
		case 8:
			if (!generatedEmail) // Nếu chưa tạo email, thực hiện cấp email trước khi thoát
			{
				printf("⚠️ Chưa tạo email cho sinh viên. Hệ thống sẽ tự động cấp email trước khi thoát!\n");

				if (!generatedStudentCode) // Nếu chưa cấp mã sinh viên
				{
					if (!sorted) // Nếu chưa sắp xếp
					{
						printf("✅ Tiến hành sắp xếp danh sách...\n");
						sortStudents();
						printf("✅ Danh sách sinh viên đã được sắp xếp\n");
					}

					printf("✅ Tiến hành cấp mã sinh viên...\n");
					generateStudentCode(); // Cấp mã sinh viên sau khi đã sắp xếp
				}

				printf("✅ Tiến hành cấp email...\n");
				generateEmail(); // Cấp email sau khi đã có mã sinh viên
			}

			createFile(); // Tạo file sau khi đã có email
			printf("🔚 Thoát chương trình.\n");
			break;
		default:
			printf("❌ Lựa chọn không hợp lệ.\n");
		}
	} while (choice != 8);
}
/* End menu */

// Kiểm tra chuỗi chỉ chứa chữ cái
int isAlphaString(const char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (!isalpha(str[i]) && str[i] != ' ') // Cho phép cả chữ cái và khoảng trắng
			return 0;
	}
	return 1;
}

// Kiểm tra năm nhuận
int isLeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Kiểm tra ngày hợp lệ
int isValidDate(int day, int month, int year)
{
	if (year < 1900 || year > 2024)
		return 0;
	if (month < 1 || month > 12)
		return 0;

	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (isLeapYear(year))
		daysInMonth[1] = 29;

	return day >= 1 && day <= daysInMonth[month - 1];
}

// Kiểm tra số lượng sinh viên thêm vào
int getValidStudentCount()
{
	int n;
	while (1)
	{
		printf("👉 Nhập số lượng sinh viên cần thêm: ");

		if (scanf("%d", &n) != 1) // Nếu nhập không phải số nguyên
		{
			printf("❌ Lỗi: Vui lòng nhập một số nguyên hợp lệ!\n");
			while (getchar() != '\n')
				; // Xóa bộ nhớ đệm
			continue;
		}

		if (n <= 0)
		{
			printf("❌ Lỗi: Số lượng sinh viên phải lớn hơn 0!\n");
			continue;
		}

		if (totalStudents + n > MAX_STUDENTS)
		{
			printf("❌ Lỗi: Số lượng sinh viên không được vượt quá %d!\n", MAX_STUDENTS);
			continue;
		}

		getchar(); // Thêm dòng này để xóa bộ nhớ đệm trước khi nhập chuỗi

		return n; // Trả về số lượng hợp lệ
	}
}

// Hàm viết hoa chữ cái đầu tiên của mỗi từ
void capitalizeWords(char *str)
{
	int len = strlen(str);
	int capitalizeNext = 1; // Cờ đánh dấu ký tự đầu tiên của mỗi từ

	for (int i = 0; i < len; i++)
	{
		if (isalpha(str[i]))
		{
			if (capitalizeNext)
			{
				str[i] = toupper(str[i]); // Viết hoa chữ cái đầu tiên
				capitalizeNext = 0;
			}
			else
			{
				str[i] = tolower(str[i]); // Chuyển các chữ khác về thường
			}
		}
		else
		{
			capitalizeNext = 1; // Khi gặp khoảng trắng hoặc ký tự đặc biệt, đánh dấu từ mới
		}
	}
}

// Hàm nhập và kiểm tra STT hợp lệ (3 chữ số, từ 001 đến 100)
int getValidStudentIndex()
{
	char input[10]; // Lưu chuỗi nhập vào
	int index;

	while (1)
	{
		printf("👉 Nhập STT sinh viên cần xóa (3 chữ số, từ 001 đến 100): ");
		scanf("%s", input);

		// Kiểm tra độ dài phải đúng 3 chữ số
		if (strlen(input) != 3 || !isdigit(input[0]) || !isdigit(input[1]) || !isdigit(input[2]))
		{
			printf("❌ Lỗi: STT phải có đúng 3 chữ số (ví dụ: 001, 002, ...)\n");
			continue;
		}

		// Chuyển chuỗi thành số nguyên
		index = atoi(input);

		// Kiểm tra trong phạm vi hợp lệ
		if (index < 1 || index > MAX_STUDENTS)
		{
			printf("❌ Lỗi: STT phải từ 001 đến 100.\n");
			continue;
		}

		return index - 1; // Trả về chỉ mục (0-based index)
	}
}

// Hàm hoán đổi chuỗi
void swapStrings(char *a, char *b)
{
	char temp[100]; // Định nghĩa bộ nhớ tạm
	strcpy(temp, a);
	strcpy(a, b);
	strcpy(b, temp);
}

// Hàm hoán đổi số nguyên
void swapInts(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// Chuyển chuỗi về chữ thường (để tìm không phân biệt hoa thường)
void toLowerCase(char *str)
{
	for (int i = 0; str[i]; i++)
		str[i] = tolower(str[i]);
}

// Hàm kiểm tra chuỗi con (substring)
bool containsSubstring(char *str, char *sub)
{
	char lowerStr[50], lowerSub[50];
	strcpy(lowerStr, str);
	strcpy(lowerSub, sub);
	toLowerCase(lowerStr);
	toLowerCase(lowerSub);
	return strstr(lowerStr, lowerSub) != NULL; // Kiểm tra chuỗi con
}