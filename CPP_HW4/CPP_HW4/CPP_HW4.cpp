#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std; // namespace std 사용

class Book {
public:
    string title;
    string author;

    Book(const string& title, const string& author)
        : title(title), author(author) {
    }
};

class BookManager {
private:
    vector<Book> books; // 책 목록 저장

    Book* findBookByTitle(const string& title)
    {
        vector<Book>::iterator itr;

        for (itr = books.begin(); itr < books.end(); itr++)
        {
            if (itr->title == title)
            {
                return &(*itr);
            }
        }
        return nullptr;
    }

    Book* findBookByAuthor(const string& author)
    {
        vector<Book>::iterator itr;

        for (itr = books.begin(); itr < books.end(); itr++)
        {
            if (itr->author == author)
            {
                return &(*itr);
            }
        }
        return nullptr;
    }

public:
    // 책 추가 메서드
    void addBook(const string& title, const string& author) {
        books.push_back(Book(title, author)); // push_back 사용
        cout << "책이 추가되었습니다: " << title << " by " << author << endl;
    }

    // 모든 책 출력 메서드
    void displayAllBooks() const {
        if (books.empty()) {
            cout << "현재 등록된 책이 없습니다." << endl;
            return;
        }

        cout << "현재 도서 목록:" << endl;
        for (size_t i = 0; i < books.size(); i++) { // 일반적인 for문 사용
            cout << "- " << books[i].title << " by " << books[i].author << endl;
        }
    }

    // 책의 제목 & 작가 getter
    Book* getBookByTitle(const string& title) { return findBookByTitle(title); }
    Book* getBookByAuthor(const string& author) { return findBookByAuthor(author); }
};
// 책 대여 관리 클래스
class BorrowManager
{
private:
    unordered_map<string, int> stock; // 비정렬 맵(key-value)

public:
    // 책 재고 초기화
    // stock변수 초기화 하는 함수임.
    // 책 추가할 때마다 같이 호출해야 함.
    void initializeStock(const Book* book, int quantity = 3)
    {
        stock.insert(make_pair(book->title, quantity));
    }

    // 책 대여
    // 책 대여하면 재고 -1 하면됨.
    void borrowBook(const string& title)
    {
        // find함수로 title인 stock 반복자 생성 없을 경우 stock.end()값 반환함
        unordered_map<string, int>::iterator itr = stock.find(title);
        
        if (itr != stock.end()) // 책이 존재하는지 체크
        {
            cout << "현재 재고 : " << itr->second << endl;
            // 재고 가 있는 경우
            if (itr->second > 0)
            {
                char choice;

                while (true)
                {
                    cout << "해당 책을 대여하시겠습니까? (Y/N): ";
                    cin >> choice;
                    choice = toupper(choice); // 소문자 입력에도 대응

                    if (choice == 'Y')
                    {
                        cout << title << " 대여가 완료되었습니다. 독서가 즐거운 경험이 되길 바랍니다." << endl;
                        itr->second--;
                        break;
                    }
                    else if (choice == 'N')
                    {
                        cout << "대여를 취소하였습니다." << endl;
                        break;
                    }
                    else
                    {
                        cout << "잘못 입력하셨습니다. 'Y' 또는 'N'을 입력해주세요." << endl;
                        // 반복 계속
                    }
                }
                cin.ignore();
            }
            else
            {
                cout << "죄송합니다. " << title << "은 현재 재고가 없습니다. 나중에 다시 확인해 주세요." << endl;
            }
        }
        else // 대여 할 책이 없음
        {
            cout << "대여할 책이 존재하지 않습니다." << endl;
        }
    }

    void returnBook(const string& title)
    {
        unordered_map<string, int>::iterator itr = stock.find(title);

        // 책이 존재 하는지 체크
        if (itr != stock.end()) // 있음
        {
            if (itr->second < 3)
            {
                char choice;
                while (true)
                {
                    cout << "해당 책을 반납하시겠습니까? (Y/N): ";
                    cin >> choice;
                    choice = toupper(choice); // 소문자 입력에도 대응

                    if (choice == 'Y')
                    {
                        cout << "반납이 완료되었습니다. 이용해 주셔서 감사합니다." << endl;
                        itr->second++;
                        break;
                    }
                    else if (choice == 'N')
                    {
                        cout << "반납을 취소하였습니다." << endl;
                        break;
                    }
                    else
                    {
                        cout << "잘못 입력하셨습니다. 'Y' 또는 'N'을 입력해주세요." << endl;
                        // 반복 계속
                    }
                }
                cin.ignore();
            }
            else // 총 개수보다 책이 많아 질 경우
            {
                cout << "이미 전부 반납된 도서입니다." << endl;
            }
        }
        else
        {
            cout << "반납할 책이 존재하지 않습니다." << endl;
        }
    }
};

int main() {
    BookManager bookManager;
    BorrowManager borrowManager;

    // 도서관 관리 프로그램의 기본 메뉴를 반복적으로 출력하여 사용자 입력을 처리합니다.
    // 프로그램 종료를 선택하기 전까지 계속 동작합니다.
    while (true) {
        cout << "\n도서관 관리 프로그램" << endl;
        cout << "1. 책 추가" << endl; // 책 정보를 입력받아 책 목록에 추가
        cout << "2. 모든 책 출력" << endl; // 현재 책 목록에 있는 모든 책 출력
        cout << "3. 책 대여" << endl; // 대여하고 싶은 책 선택하는 안내창 출력
        cout << "4. 책 반납" << endl; // 대여한 책 반납 안내창 출력
        cout << "3. 종료" << endl; // 프로그램 종료
        cout << "선택: ";

        int choice; // 사용자의 메뉴 선택을 저장
        cin >> choice;

        if (choice == 1) {
            // 1번 선택: 책 추가
            // 사용자로부터 책 제목과 저자명을 입력받아 BookManager에 추가합니다.
            string title, author;
            cout << "책 제목: ";
            cin.ignore(); // 이전 입력의 잔여 버퍼를 제거
            getline(cin, title); // 제목 입력 (공백 포함)
            cout << "책 저자: ";
            getline(cin, author); // 저자명 입력 (공백 포함)
            bookManager.addBook(title, author); // 입력받은 책 정보를 추가
            borrowManager.initializeStock(bookManager.getBookByTitle(title)); // 책 재고 초기화( 기본값 3권 )
        }
        else if (choice == 2) {
            // 2번 선택: 모든 책 출력
            // 현재 BookManager에 저장된 책 목록을 출력합니다.
            bookManager.displayAllBooks();
        }
        else if (choice == 3)
        {
            // 3번 선택: 책 대여
            // 1. 대여 할 책 제목 입력
            // 2. 재고 출력 or 재고 없음만 알려줄까?
            bool isLoop = 1;
            cin.ignore();
            while (isLoop)
            {
                string title;
                cout << "\n대여하실 책 제목을 입력해주세요.(뒤로가기 : 0)" << endl;
                cout << "책 제목: ";
                getline(cin, title);
                
                // 뒤로가기
                if (title == "0")
                {
                    isLoop = false;
                    break;
                }
                
                // 책 대여 함수 호출
                borrowManager.borrowBook(title);
            }
        }
        else if (choice == 4)
        {
            // 4번 선택: 책 반납
            // 반납할 책 제목 입력
            // 재고가 3이상일 경우 또는 책 목록에 없는 책인 경우 실패 안내
            bool isLoop = 1;
            cin.ignore();
            while (isLoop)
            {
                string title;
                cout << "\n반납하실 책 제목을 입력해주세요.(뒤로가기 : 0)" << endl;
                cout << "책 제목: ";
                getline(cin, title);

                // 뒤로가기
                if (title == "0")
                {
                    isLoop = false;
                    break;
                }

                borrowManager.returnBook(title);
            }

        }
        else if (choice == 5) {
            // 3번 선택: 종료
            // 프로그램을 종료하고 사용자에게 메시지를 출력합니다.
            cout << "프로그램을 종료합니다." << endl;
            break; // while 루프 종료
        }
        else {
            // 잘못된 입력 처리
            // 메뉴에 없는 번호를 입력했을 경우 경고 메시지를 출력합니다.
            cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
        }
    }

    return 0; // 프로그램 정상 종료
}