#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define STUDENT_ID_LIMIT 11
#define STUDENT_CODE_LIMIT 9
#define STUDENT_EMAIL_LIMIT 30
#define STUDENT_FIRST_NAME_LIMIT 20
#define STUDENT_LAST_NAME_LIMIT 20
#define STUDENT_GENDER_LIMIT 5
#define STUDENT_ADDRESS_LIMIT 30
#define STUDENT_EMAIL_LIMIT 30

#define MAX_STUDENTS 100
#define MAX_FILENAME_LENGTH 50

/* Mảng song song lưu thông tin sinh viên */
char studentID[MAX_STUDENTS][STUDENT_ID_LIMIT];			// ID sinh viên (tự động tăng)
char studentCode[MAX_STUDENTS][STUDENT_CODE_LIMIT];		// Mã số sinh viên
char studentEmail[MAX_STUDENTS][STUDENT_EMAIL_LIMIT];	// Email sinh viên
char firstName[MAX_STUDENTS][STUDENT_FIRST_NAME_LIMIT]; // Họ lót
char lastName[MAX_STUDENTS][STUDENT_LAST_NAME_LIMIT];	// Tên
char gender[MAX_STUDENTS][STUDENT_GENDER_LIMIT];		// Giới tính
char address[MAX_STUDENTS][STUDENT_ADDRESS_LIMIT];		// Địa chỉ
int birthDay[MAX_STUDENTS];								// Ngày sinh
int birthMonth[MAX_STUDENTS];							// Tháng sinh
int birthYear[MAX_STUDENTS];							// Năm sinh

/* Biến toàn cục để lưu tên file */
char fileName[MAX_FILENAME_LENGTH]; // Tên file

/* Các thông tin chung của lớp */
int totalStudents = 0; // Số lượng sinh viên hiện tại
int academicYear;	   // Niên khóa
int majorCode;		   // Mã ngành
bool sorted = 0;	   // Đánh dấu danh sách đã sắp xếp chưa

/* Biến trạng thái */
bool generatedStudentCode = 0; // Đánh dấu đã cấp mã SV chưa
bool generatedEmail = 0;	   // Đánh dấu đã cấp email chưa

/* Khai báo các hàm chính */

void addStudent();
void saveStudentToFile();
void saveStudentsToFile();
void sortStudents();
void deleteStudent(int index);
void deleteStudentByName();
void deleteStudentByCode();
void deleteStudentByNameOrStudentCode();
int findStudentIndexByID(char *studentID);
void searchStudent();
void generateStudentCode();
void generateEmail();
void printStudents();
void inputFile();
bool readFile(const char *fileName);
void createFile();
void exitFile();
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
int isValidNumber(const char *str);
void clearInputBuffer();

/* Main */
int main()
{
	printf(" /\\_/\\      \n");
	printf("( o.o )-------------------------------------------------------------------------------------------------------------+\n");
	printf(" > ^ <                                                ĐỒ ÁN TIN HỌC                                                 |\n");
	printf(" |                                              ỨNG DỤNG QUẢN LÝ SINH VIÊN                                          |\n");
	printf(" +------------------------------------------------------------------------------------------------------------------+\n");
	printf(" |                                                     THÀNH VIÊN                                                   |\n");
	printf(" |                                              1. NGUYỄN TRUNG NGHĨA                                               |\n");
	printf(" |                                              2. TRƯƠNG TOÀN MINH NHẬT                                            |\n");
	printf(" |                                              3. VÕ UYÊN THƯ                                                      |\n");
	printf(" |                                              4. TRƯƠNG HOÀNG MỸ ÁI                                               |\n");
	printf(" +------------------------------------------------------------------------------------------------------------------+\n");

	menu(); // Gọi menu để thao tác với file
	return 0;
}

/* Thêm sinh viên */
void addStudent()
{
	char tempFirstName[STUDENT_FIRST_NAME_LIMIT],
		tempLastName[STUDENT_LAST_NAME_LIMIT],
		tempGender[STUDENT_GENDER_LIMIT],
		tempAddress[STUDENT_ADDRESS_LIMIT];
	int tempBirthDay,
		tempBirthMonth,
		tempBirthYear;

	// Sinh ID tự động
	sprintf(studentID[totalStudents], "%03d", totalStudents + 1);

	// Nhập họ lót
	while (1)
	{
		printf("👉 Nhập họ lót: ");
		fgets(tempFirstName, sizeof(tempFirstName), stdin);
		tempFirstName[strcspn(tempFirstName, "\n")] = '\0';

		if (strlen(tempFirstName) > 0 && isAlphaString(tempFirstName))
		{
			capitalizeWords(tempFirstName);
			break;
		}
		printf("❌ Họ lót không hợp lệ! Chỉ được nhập chữ cái. Vui lòng nhập lại.\n");
	}

	// Nhập tên
	while (1)
	{
		printf("👉 Nhập tên: ");
		fgets(tempLastName, sizeof(tempLastName), stdin);
		tempLastName[strcspn(tempLastName, "\n")] = '\0';

		if (strlen(tempLastName) > 0 && isAlphaString(tempLastName))
		{
			capitalizeWords(tempLastName);
			break;
		}
		printf("❌ Tên không hợp lệ! Chỉ được nhập chữ cái. Vui lòng nhập lại.\n");
	}

	// Nhập ngày sinh
	while (1)
	{
		char dateInput[11]; // Lưu trữ chuỗi nhập vào
		printf("📅 Nhập ngày sinh (DD/MM/YYYY): ");

		scanf(" %[^\n]", dateInput); // Đọc toàn bộ dòng

		// Kiểm tra định dạng "DD/MM/YYYY"
		if (sscanf(dateInput, "%d/%d/%d", &tempBirthDay, &tempBirthMonth, &tempBirthYear) != 3)
		{
			printf("❌ Định dạng không hợp lệ! Hãy nhập đúng định dạng DD/MM/YYYY.\n");
			continue; // Quay lại nhập lại
		}

		// Kiểm tra xem ngày nhập vào có hợp lệ không
		if (!isValidDate(tempBirthDay, tempBirthMonth, tempBirthYear))
		{
			printf("❌ Ngày sinh không hợp lệ! Hãy nhập ngày thực tế.\n");
			continue; // Quay lại nhập lại
		}

		// Nếu cả hai điều kiện đúng, thoát vòng lặp
		break;
	}

	// Nhập giới tính
	while (1)
	{
		printf("👉 Chọn giới tính (0 - Nu, 1 - Nam): ");
		int choice;
		if (scanf("%d", &choice) == 1 && (choice == 0 || choice == 1))
		{
			strcpy(tempGender, (choice == 0) ? "Nu" : "Nam");
			while (getchar() != '\n')
				;
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
		fgets(tempAddress, sizeof(tempAddress), stdin);
		tempAddress[strcspn(tempAddress, "\n")] = '\0';

		if (strlen(tempAddress) > 0)
		{
			capitalizeWords(tempAddress);
			break;
		}
		printf("❌ Địa chỉ không được để trống! Vui lòng nhập lại.\n");
	}

	int pos = totalStudents; // Vị trí chèn sẽ mặc định là cuối danh sách

	// Xác định vị trí chèn nếu danh sách đã được sắp xếp
	if (sorted == 1)
	{
		for (int i = 0; i < totalStudents; i++)
		{
			if (strcmp(tempLastName, lastName[i]) < 0 ||
				(strcmp(tempLastName, lastName[i]) == 0 && strcmp(tempFirstName, firstName[i]) < 0))
			{
				pos = i;
				break;
			}
		}

		// Dời dữ liệu để chèn đúng vị trí
		for (int j = totalStudents; j > pos; j--)
		{
			strcpy(firstName[j], firstName[j - 1]);
			strcpy(lastName[j], lastName[j - 1]);
			strcpy(gender[j], gender[j - 1]);
			strcpy(address[j], address[j - 1]);

			birthDay[j] = birthDay[j - 1];
			birthMonth[j] = birthMonth[j - 1];
			birthYear[j] = birthYear[j - 1];
		}
	}

	// Gán dữ liệu vào vị trí chèn
	strcpy(firstName[pos], tempFirstName);
	strcpy(lastName[pos], tempLastName);
	strcpy(gender[pos], tempGender);
	strcpy(address[pos], tempAddress);
	birthDay[pos] = tempBirthDay;
	birthMonth[pos] = tempBirthMonth;
	birthYear[pos] = tempBirthYear;

	// Cập nhật số lượng sinh viên
	totalStudents++;
	generatedStudentCode = 0;
	generatedEmail = 0;

	// Ghi vào file
	saveStudentToFile();

	printf("✅ Đã thêm sinh viên thành công!\n");
}

/* Hàm ghi thông tin sinh viên vào file cho hàm add student */
void saveStudentToFile()
{
	FILE *file;

	if (sorted == 1)
	{
		// Nếu đã sắp xếp, ghi lại toàn bộ file
		file = fopen(fileName, "w");
		if (file == NULL)
		{
			printf("❌ Không thể mở file để ghi dữ liệu!\n");
			return;
		}

		// Ghi thông tin niên khóa, mã ngành
		fprintf(file, "%d,%d\n", academicYear, majorCode);

		// Ghi tổng số sinh viên đã cập nhật
		fprintf(file, "%d\n", totalStudents);

		// Ghi trạng thái đã sắp xếp chưa?
		fprintf(file, "%d\n", sorted);

		// Ghi trạng thái đã cấp mã sinh viên chưa?
		fprintf(file, "%d\n", generatedStudentCode);

		// Ghi toàn bộ danh sách sinh viên
		for (int i = 0; i < totalStudents; i++)
		{
			fprintf(file, "%s,%s,%s,%02d,%02d,%04d,%s,%s\n",
					studentID[i], firstName[i], lastName[i],
					birthDay[i], birthMonth[i], birthYear[i],
					gender[i], address[i]);
		}
		fclose(file);
	}
	else
	{
		// Mở file với "r+" để cập nhật số lượng sinh viên
		file = fopen(fileName, "r+");
		if (file == NULL)
		{
			printf("❌ Không thể mở file để cập nhật số lượng sinh viên!\n");
			return;
		}

		// Bỏ qua dòng đầu tiên (niên khóa, mã ngành)
		char line[256];
		fgets(line, sizeof(line), file); // bỏ dòng 1

		// Ghi đè dòng thứ 2
		fseek(file, ftell(file), SEEK_SET); // đảm bảo vị trí con trỏ chính xác
		fprintf(file, "%d\n", totalStudents);

		fclose(file);

		// Mở file với "a" để ghi thêm sinh viên mới vào cuối
		file = fopen(fileName, "a");
		if (file == NULL)
		{
			printf("❌ Không thể mở file để ghi thêm dữ liệu!\n");
			return;
		}

		// Ghi sinh viên mới vào cuối file
		fprintf(file, "%s,%s,%s,%02d,%02d,%04d,%s,%s\n",
				studentID[totalStudents - 1], firstName[totalStudents - 1], lastName[totalStudents - 1],
				birthDay[totalStudents - 1], birthMonth[totalStudents - 1], birthYear[totalStudents - 1],
				gender[totalStudents - 1], address[totalStudents - 1]);

		fclose(file);
	}
}

/* Hàm ghi danh sách sinh viên vào file */
void saveStudentsToFile()
{
	FILE *file = fopen(fileName, "w"); // Ghi lại toàn bộ file

	if (!file)
	{
		printf("❌ Lỗi: Không thể mở file để ghi dữ liệu!\n");
		return;
	}

	// Ghi thông tin niên khóa, mã ngành
	fprintf(file, "%d,%d\n", academicYear, majorCode);

	// Ghi tổng số sinh viên đã cập nhật
	fprintf(file, "%d\n", totalStudents);

	// Ghi trạng thái đã sắp xếp
	fprintf(file, "%d\n", sorted);

	// Ghi trạng thái đã cấp mã sinh viên
	fprintf(file, "%d\n", 0);

	// Ghi toàn bộ danh sách sinh viên
	for (int i = 0; i < totalStudents; i++)
	{
		fprintf(file, "%s,%s,%s,%02d,%02d,%04d,%s,%s\n",
				studentID[i], firstName[i], lastName[i],
				birthDay[i], birthMonth[i], birthYear[i],
				gender[i], address[i]);
	}
	fclose(file);
}

/* Sắp xếp danh sách sinh viên theo họ lót và tên (bubble sort tối ưu) */
void sortStudents()
{
	if (totalStudents <= 1)
		return; // Không cần sắp xếp nếu danh sách trống hoặc có 1 sinh viên

	bool swapped; // Kiểm tra xem có hoán đổi nào không

	for (int i = 0; i < totalStudents - 1; i++)
	{
		swapped = false; // Mặc định không có hoán đổi

		for (int j = 0; j < totalStudents - i - 1; j++)
		{
			// So sánh theo lastName trước, nếu trùng thì so sánh firstName
			if (strcmp(lastName[j], lastName[j + 1]) > 0 ||
				(strcmp(lastName[j], lastName[j + 1]) == 0 &&
				 strcmp(firstName[j], firstName[j + 1]) > 0))
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

	sorted = 1;			  // Khi vòng lặp kết thúc, danh sách đã sắp xếp
	saveStudentsToFile(); // Ghi danh sách sau khi sắp xếp vào file
}

/* Xóa sinh viên khỏi mảng song */
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

	totalStudents--;	  // Giảm tổng số sinh viên
	saveStudentsToFile(); // Lưu danh sách vào file sau khi xóa
}

/* Xóa sinh viên theo họ lót và tên */
void deleteStudentByName()
{
	char input[50];
	int foundIndexes[MAX_STUDENTS], foundCount = 0;
	int choice;

	while (1) // Lặp lại nhập nếu không tìm thấy sinh viên
	{
		printf("\n👉 Nhập họ lót và tên cần xóa: ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;

		foundCount = 0;
		for (int i = 0; i < totalStudents; i++)
		{
			char fullName[50];
			snprintf(fullName, sizeof(fullName), "%s %s", firstName[i], lastName[i]);

			if (containsSubstring(fullName, input) != NULL)
				foundIndexes[foundCount++] = i;
		}

		if (foundCount == 0)
		{
			printf("❌ Không tìm thấy sinh viên nào có tên \"%s\". Vui lòng nhập lại!\n", input);
			continue; // Quay lại nhập lại
		}
		break; // Thoát khỏi vòng lặp nhập nếu tìm thấy sinh viên
	}

	if (foundCount == 1) // ✅ Nếu chỉ tìm thấy 1 sinh viên, hỏi xác nhận ngay
	{
		int deleteIdx = foundIndexes[0];

		printf("\n🔸 Chỉ tìm thấy 1 sinh viên: \"%s %s\".\n",
			   firstName[deleteIdx], lastName[deleteIdx]);

		printf("❓ Bạn có chắc chắn muốn xóa sinh viên này? (✅ 1: Có, ❌ 0: Không): ");
		scanf("%d", &choice);
		getchar();

		if (choice == 1)
		{
			deleteStudent(deleteIdx);
			saveStudentsToFile();
			printf("✅ Đã xóa sinh viên thành công!\n");
		}
		else
		{
			printf("🔙 Hủy xóa.\n");
		}
		return;
	}

	while (1) // ✅ Nếu có nhiều sinh viên, yêu cầu chọn số thứ tự để xóa
	{
		printf("\n🔹 Danh sách sinh viên tìm thấy:\n");
		printf("%-5s %-18s %-11s %-12s %-15s %-20s", "", "Họ lót", "Tên", "Ngày sinh", "Giới tính", "Địa chỉ");
		if (generatedStudentCode)
			printf(" %-12s", "MSSV");
		if (generatedEmail)
			printf(" %-25s", "Email");
		printf("\n");
		printf("---------------------------------------------------------------------------------------------\n");

		for (int i = 0; i < foundCount; i++)
		{
			int idx = foundIndexes[i];
			printf("%4d. %-15s %-10s %02d/%02d/%02d  %-12s %-15s",
				   i + 1, firstName[idx], lastName[idx],
				   birthDay[idx], birthMonth[idx], birthYear[idx],
				   gender[idx], address[idx]);

			if (generatedStudentCode)
				printf(" %-12s", studentCode[idx]);
			if (generatedEmail)
				printf(" %-25s", studentEmail[idx]);

			printf("\n");
		}

		printf("\n👉 Nhập số thứ tự sinh viên muốn xóa (🔙 Nhấn 0 để quay lại): ");
		scanf("%d", &choice);
		getchar();

		if (choice == 0)
			return; // Quay lại nhập họ và tên

		if (choice < 1 || choice > foundCount)
		{
			printf("❌ Số thứ tự không hợp lệ! Vui lòng nhập lại.\n");
			continue;
		}

		int deleteIdx = foundIndexes[choice - 1];
		printf("\n❓ Bạn có chắc chắn muốn xóa sinh viên \"%s %s\"? (✅ 1: Có, ❌ 0: Không): ",
			   firstName[deleteIdx], lastName[deleteIdx]);
		scanf("%d", &choice);
		getchar();

		if (choice == 0)
		{
			printf("🔙 Hủy xóa. Quay lại danh sách sinh viên.\n");
			continue;
		}

		if (choice == 1)
		{
			deleteStudent(deleteIdx);
			saveStudentsToFile();
			printf("✅ Đã xóa sinh viên thành công!\n");
			return;
		}

		printf("❌ Lựa chọn không hợp lệ. Vui lòng nhập lại.\n");
	}
}

/* Xóa sinh viên theo MSSV */
void deleteStudentByCode()
{
	char input[50];
	int choice, attempt = 0;

	while (1)
	{
		printf("👉 Nhập MSSV cần xóa (🔙 Nhấn 0 để thoát): ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;

		if (strcmp(input, "0") == 0) // Người dùng muốn thoát
		{
			printf("🔙 Hủy thao tác xóa sinh viên.\n");
			return;
		}

		int index = -1;
		for (int i = 0; i < totalStudents; i++)
		{
			if (strcmp(studentCode[i], input) == 0)
			{
				index = i;
				break;
			}
		}

		if (index == -1)
		{
			printf("❌ Không tìm thấy sinh viên với MSSV: %s.\n", input);
			attempt++;
			if (attempt >= 3)
			{
				printf("🚨 Bạn đã nhập sai 3 lần. Quay lại menu chính.\n");
				return;
			}
			continue;
		}

		while (1)
		{
			printf("\n❓ Bạn có chắc chắn muốn xóa sinh viên \"%s %s\"? (✅ 1: Có, ❌ 0: Không): ",
				   firstName[index], lastName[index]);
			scanf("%d", &choice);
			getchar();

			if (choice == 0)
			{
				printf("🔙 Hủy thao tác xóa sinh viên.\n");
				return;
			}

			if (choice == 1)
			{
				deleteStudent(index);
				saveStudentsToFile();
				printf("✅ Đã xóa sinh viên thành công!\n");
				return;
			}

			printf("❌ Lựa chọn không hợp lệ! Vui lòng nhập lại.\n");
		}
	}
}

/* Xóa sinh viên theo tên và họ lót (hoặc theo MSSV)*/
void deleteStudentByNameOrStudentCode()
{
	char input[10]; // Chuỗi để đọc đầu vào
	int choice;

	while (1)
	{
		printf("\n🔹 Chọn phương thức xóa:\n");
		printf("✨ 1. Xóa bằng họ lót và tên\n");
		if (generatedStudentCode)
			printf("✨ 2. Xóa bằng MSSV\n");
		printf("✨ 0. Thoát\n");
		printf("👉 Nhập lựa chọn: ");

		// Đọc đầu vào thay vì dùng scanf
		if (!fgets(input, sizeof(input), stdin)) // Kiểm tra lỗi khi nhập
		{
			printf("❌ Đã xảy ra lỗi khi nhập. Vui lòng thử lại!\n");
			continue;
		}

		input[strcspn(input, "\n")] = '\0'; // Xóa ký tự xuống dòng

		if (!isValidNumber(input)) // Kiểm tra nếu nhập không phải số
		{
			printf("❌ Lựa chọn không hợp lệ. Vui lòng nhập lại!\n");
			continue;
		}

		choice = atoi(input); // Chuyển chuỗi thành số nguyên

		if (choice == 0)
		{
			printf("🔙 Hủy thao tác xóa.\n");
			return;
		}
		else if (choice == 1)
			deleteStudentByName();
		else if (choice == 2 && generatedStudentCode)
			deleteStudentByCode();
		else
			printf("❌ Lựa chọn không hợp lệ. Vui lòng nhập lại!\n");
	}
}

// Hàm hỗ trợ tìm sinh viên theo MSSV
int findStudentIndexByID(char *studentID)
{
	for (int i = 0; i < totalStudents; i++)
	{
		if (strcmp(studentCode[i], studentID) == 0)
			return i;
	}
	return -1;
}

/* Tìm kiếm theo họ lót và tên sinh viên (hoặc theo MSSV) */
void searchStudent()
{
	char input[50]; // Chứa từ khóa tìm kiếm
	int found;		// Cờ kiểm tra có tìm thấy sinh viên không

	while (1)
	{
		found = 0; // Reset biến kiểm tra mỗi lần nhập từ khóa
		printf("\n🔍 Nhập từ khóa tìm kiếm (Họ lót và tên%s) (🔙 Nhấn 0 để thoát): ", generatedStudentCode ? " hoặc MSSV" : "");
		scanf(" %[^\n]", input); // Cho phép nhập cả họ + tên có dấu cách

		if (strcmp(input, "0") == 0) // Nếu người dùng nhập 0, thoát khỏi tìm kiếm
		{
			printf("🔙 Thoát tìm kiếm.\n");
			return;
		}

		for (int i = 0; i < totalStudents; i++)
		{
			char fullName[50]; // Ghép Họ lót + Tên
			snprintf(fullName, sizeof(fullName), "%s %s", firstName[i], lastName[i]);

			// Kiểm tra input có xuất hiện trong fullName hoặc MSSV không
			if (containsSubstring(fullName, input) ||
				(generatedStudentCode &&
				 containsSubstring(studentCode[i], input)))
			{
				if (!found) // Chỉ in tiêu đề bảng một lần nếu có kết quả
				{
					printf("\n🔎 Kết quả tìm kiếm:\n");
					printf("%-5s %-18s %-11s %-12s %-15s %-20s", "ID", "Họ lót", "Tên", "Ngày sinh", "Giới tính", "Địa chỉ");
					if (generatedStudentCode)
						printf(" %-12s", "MSSV"); // Thêm MSSV nếu đã tạo
					if (generatedEmail)
						printf(" %-25s", "Email"); // Thêm Email nếu đã tạo
					printf("\n");
					printf("---------------------------------------------------------------------------------------------\n");
				}

				// In thông tin sinh viên tìm thấy
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

		// Nếu không tìm thấy sinh viên nào, yêu cầu nhập lại
		if (!found)
		{
			printf("\n🔎 Kết quả tìm kiếm:");
			printf("❌ Không tìm thấy sinh viên! Vui lòng thử lại.\n");
		}
	}
}

/* Cấp MSSV */
void generateStudentCode()
{
	FILE *file;
	char line[256];

	// Nếu chưa cấp mã sinh viên (generatedStudentCode == 0), cấp mã và ghi trạng thái vào file
	if (generatedStudentCode == 0)
	{
		// Cấp MSSV cho sinh viên
		for (int i = 0; i < totalStudents; i++)
		{
			sprintf(studentCode[i], "%03d%02d%03d", majorCode, academicYear, i + 1);
		}

		// Cập nhật trạng thái đã cấp mã sinh viên
		generatedStudentCode = 1;

		// Mở file với "r+" để cập nhật trạng thái đã cấp mã sinh viên
		file = fopen(fileName, "r+");
		if (file == NULL)
		{
			printf("❌ Không thể mở file để cập nhật trạng thái đã cấp mã sinh viên!\n");
			return;
		}

		// Bỏ qua ba dòng đầu tiên (niên khóa, mã ngành)
		for (int i = 0; i < 3; i++)
		{
			fgets(line, sizeof(line), file); // bỏ qua ba dòng đầu
		}

		// Đảm bảo con trỏ tệp ở đúng vị trí để ghi đè dòng thứ 4
		fseek(file, ftell(file), SEEK_SET);

		// Ghi đè dòng thứ 4 với trạng thái đã cấp mã sinh viên
		fprintf(file, "%d\n", generatedStudentCode);

		// Đóng file sau khi thao tác xong
		fclose(file);

		printf("✅ Đã tạo mã sinh viên thành công và cập nhật trạng thái!\n");
	}
	else
	{
		// Nếu đã cấp mã sinh viên, chỉ cần cấp mã cho các sinh viên mới
		for (int i = 0; i < totalStudents; i++)
		{
			sprintf(studentCode[i], "%03d%02d%03d", majorCode, academicYear, i + 1);
		}
	}
};

/* Cấp email */
void generateEmail()
{
	for (int i = 0; i < totalStudents; i++)
	{
		sprintf(studentEmail[i], "%s@sv.dut.udn.vn", studentCode[i]);
	}

	generatedEmail = 1;
	printf("✅ Đã tạo email sinh viên thành công!\n");
};

/* In danh sách học sinh */
void printStudents()
{
	printf("📌 Thông tin chung:\n");
	printf("   👥 Số lượng sinh viên: %d\n", totalStudents);
	printf("   🏫 Niên khóa: %d\n", academicYear);
	printf("   📚 Mã ngành: %d\n", majorCode);
	printf("   🔄 Đã sắp xếp: %s\n", sorted ? "✅ Có" : "❌ Chưa");
	printf("   🎓 Đã cấp mã sinh viên: %s\n", generatedStudentCode ? "✅ Có" : "❌ Chưa");
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

/* Nhập tên file */
void inputFile()
{
	FILE *file;
	while (1) // Lặp vô hạn đến khi nhập đúng file
	{

		printf("📝 Nhập tên file cần thao tác: ");

		fgets(fileName, sizeof(fileName), stdin); // Đọc cả dòng để tránh lỗi bộ nhớ

		// Xóa ký tự '\n' nếu có
		fileName[strcspn(fileName, "\n")] = 0;

		// Kiểm tra nếu nhập rỗng thì yêu cầu nhập lại
		if (strlen(fileName) == 0)
		{
			printf("❌ Tên file không được để trống. Vui lòng nhập lại.\n");
			continue;
		}

		// Kiểm tra file có tồn tại không
		file = fopen(fileName, "r");
		if (file == NULL)
		{
			printf("❌ Không tìm thấy file '%s'. Vui lòng nhập lại.\n", fileName);
			continue; // Yêu cầu nhập lại
		}

		// Kiểm tra file có thể đọc được không
		char check;
		if (fscanf(file, "%c", &check) != 1) // Đọc thử một ký tự
		{
			printf("❌ File '%s' không đúng định dạng hoặc bị lỗi. Vui lòng nhập lại.\n", fileName);
			fclose(file);
			continue;
		}

		fclose(file); // Đóng file sau kiểm tra

		// Đọc dữ liệu file
		if (readFile(fileName))
		{
			printf("✅ Đọc file '%s' thành công! \n", fileName);
			break; // Thoát vòng lặp khi file hợp lệ
		}
		else
		{
			printf("❌ Lỗi khi đọc file. Vui lòng nhập lại.\n");
		}
	}
}

/* Đọc file để lấy dữ liệu đầu vào */
bool readFile(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("❌ Không thể mở file %s!\n", filename);
		return false;
	}

	// Đọc dòng 1: Niên khóa, mã ngành
	if (fscanf(file, "%d,%d\n", &academicYear, &majorCode) != 2)
	{
		printf("❌ Lỗi khi đọc niên khóa và mã ngành!\n");
		fclose(file);
		return false;
	}

	// Đọc dòng 2: Số lượng sinh viên
	if (fscanf(file, "%d\n", &totalStudents) != 1)
	{
		printf("❌ Lỗi khi đọc số lượng sinh viên!\n");
		fclose(file);
		return false;
	}

	// Kiểm tra số lượng sinh viên hợp lệ
	if (totalStudents > MAX_STUDENTS)
	{
		printf("❌ Lỗi: Số lượng sinh viên vượt quá giới hạn (%d)!\n", MAX_STUDENTS);
		fclose(file);
		return false;
	}

	// Đọc dòng 3: Trạng thái sắp xếp
	if (fscanf(file, "%d\n", &sorted) != 1)
	{
		printf("❌ Lỗi khi đọc trạng thái sắp xếp!\n");
		fclose(file);
		return false;
	}

	// Đọc dòng 4: Trạng thái đã tạo MSSV
	if (fscanf(file, "%d\n", &generatedStudentCode) != 1)
	{
		printf("❌ Lỗi khi đọc trạng thái đã tạo MSSV!\n");
		fclose(file);
		return false;
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
			fclose(file);
			return false;
		}
	}

	// Nếu trạng thái đã tạo MSSV là 1, thì đọc mã sinh viên từ file và cấp mã sinh viên
	if (generatedStudentCode == 1)
	{
		generateStudentCode();
	}

	fclose(file);
	return true; // Đọc file thành công
}

/* Tạo file đầu ra */
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

/* Thoát khỏi file đang làm việc */
void exitFile()
{
	printf("🔚 Đã thoát khỏi file %s\n", fileName);
	memset(fileName, 0, sizeof(fileName)); // Xóa nội dung fileName

	// Xóa bộ đệm stdin để tránh lỗi nhập rỗng
	while (getchar() != '\n')
		;

	inputFile(); // Yêu cầu nhập file mới
}

/* Menu */
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

		if (scanf("%d", &choice) != 1) // Kiểm tra nếu nhập sai
		{
			printf("❌ Lỗi: Vui lòng nhập số!\n");
			while (getchar() != '\n')
				; // Xóa bộ đệm đầu vào
			continue;
		}

		// Xóa ký tự '\n' sau khi scanf đúng
		while (getchar() != '\n')
			;

		switch (choice)
		{
		case 1:
		{
			inputFile(); // Chọn file

			char choice;
			do
			{
				if (totalStudents == MAX_STUDENTS) // Kiểm tra lại số lượng sinh viên
				{
					printf("❌ Không thể thêm, danh sách đã đủ %d sinh viên!\n", MAX_STUDENTS);
					break;
				}

				addStudent(); // Gọi hàm thêm sinh viên

				// Hỏi người dùng có muốn tiếp tục hay không
				while (1)
				{
					printf("📌 Bạn có muốn tiếp tục thêm sinh viên không? (Y/N): ");
					scanf(" %c", &choice);

					// Xóa bộ đệm ngay sau khi nhập, bất kể đúng hay sai
					while (getchar() != '\n')
						;

					// Chấp nhận cả chữ hoa và chữ thường
					if (choice == 'Y' || choice == 'y' || choice == 'N' || choice == 'n')
						break; // Nhập đúng thì thoát vòng lặp hỏi lại
					else
						printf("❌ Lựa chọn không hợp lệ! Vui lòng nhập lại.\n");
				}
			} while (choice == 'Y' || choice == 'y'); // Tiếp tục nếu nhập Y/y
			break;
		}
		case 2:
			inputFile(); // Chọn file
			if (!sorted) // Nếu danh sách chưa sắp xếp
			{
				sortStudents();
				printf("✅ Đã sắp xếp thành công\n");
			}
			else
				printf("✅ Đã sắp xếp rồi, không cần sắp xếp nữa\n");
			break;
		case 3:
			inputFile(); // Chọn file
			deleteStudentByNameOrStudentCode();
			break;
		case 4:
			inputFile(); // Chọn file
			searchStudent();
			break;
		case 5:
			inputFile(); // Chọn file
			if (!sorted)
			{
				printf("❌ Danh sách chưa được sắp xếp. Vui lòng sắp xếp trước!\n");
				break;
			}

			if (totalStudents != MAX_STUDENTS)
			{
				printf("❌ Danh sách chưa đủ %d sinh viên. Vui lòng nhập thêm!\n", MAX_STUDENTS);
				break;
			}

			if (generatedStudentCode)
			{
				printf("✅ Đã cấp mã sinh viên rồi, không cần cấp lại!\n");
				break;
			}

			generateStudentCode();
			break;
		case 6:
			inputFile(); // Chọn file
			if (generatedStudentCode)
			{
				generateEmail();
				createFile();
			}
			else
				printf("❌ Chưa cấp mã sinh viên. Không thể tạo email!\n");
			break;
		case 7:
			inputFile(); // Chọn file
			printStudents();
			break;
		case 8:
			printf("🔚 Thoát chương trình.\n");
			break;
		default:
			printf("❌ Lựa chọn không hợp lệ.\n");
		}
	} while (choice != 8);
}

/* Kiểm tra chuỗi chỉ chứa chữ cái */
int isAlphaString(const char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (!isalpha(str[i]) && str[i] != ' ') // Cho phép cả chữ cái và khoảng trắng
			return 0;
	}
	return 1;
}

/* Kiểm tra năm nhuận */
int isLeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/* Kiểm tra ngày hợp lệ */
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

/* Kiểm tra số lượng sinh viên thêm vào */
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

/* Hàm viết hoa chữ cái đầu tiên của mỗi từ */
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

/* Hàm nhập và kiểm tra STT hợp lệ (3 chữ số, từ 001 đến 100) */
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

/* Hàm hoán đổi chuỗi */
void swapStrings(char *a, char *b)
{
	char temp[100]; // Định nghĩa bộ nhớ tạm
	strcpy(temp, a);
	strcpy(a, b);
	strcpy(b, temp);
}

/* Hàm hoán đổi số nguyên */
void swapInts(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

/* Chuyển chuỗi về chữ thường (để tìm không phân biệt hoa thường) */
void toLowerCase(char *str)
{
	for (int i = 0; str[i]; i++)
		str[i] = tolower(str[i]);
}

/* Hàm kiểm tra chuỗi con (substring) */
bool containsSubstring(char *str, char *sub)
{
	char lowerStr[50], lowerSub[50];
	strcpy(lowerStr, str);
	strcpy(lowerSub, sub);
	toLowerCase(lowerStr);
	toLowerCase(lowerSub);
	return strstr(lowerStr, lowerSub) != NULL; // Kiểm tra chuỗi con
}

/* Kiểm tra xem chuỗi có phải là số nguyên hợp lệ không */
int isValidNumber(const char *str)
{
	if (str[0] == '\0') // Kiểm tra nếu chuỗi rỗng
		return 0;

	for (int i = 0; str[i] != '\0'; i++)
	{
		if (!isdigit(str[i])) // Nếu có ký tự không phải số
			return 0;
	}
	return 1;
}

/* Hàm xóa bộ nhớ đệm stdin */
void clearInputBuffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		; // Đọc hết ký tự thừa
}