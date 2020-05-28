#include <iostream>
#include <math.h>

using namespace std;

struct Time_hall {
    int number_hall;
    int hour;
    int minute;
};

struct Movie {
    string name;
    Time_hall time_hall;
};

inline bool operator==(const Movie &first, const Movie &second) {
    return (first.time_hall.minute == second.time_hall.minute && first.time_hall.hour == second.time_hall.hour &&
            first.time_hall.number_hall == second.time_hall.number_hall);
}

class HashTab {
private:

    unsigned int max_size = 20;
    unsigned int curr_size;
    Movie *info_film;
    bool *status;

    int length_of_number(int number) {
        int result = 0;
        while (number != 0) {
            number /= 10;
            result++;
        }
        return result;
    }

    int getKey(const Time_hall &time) {

        int key = time.minute + time.hour * (int) pow(10, length_of_number(time.minute));
        key += time.number_hall * (int) pow(10, key);

        return key;
    }

    int hash1(const Time_hall &time) {
        int key = getKey(time);
        return key % max_size;
    }


    int hash2(int key, int counter) {
        return (key + counter * counter) % max_size;
    }

    void ReHash(int pos, int i) {

        pos = hash2(pos, ++i);
        while (status[pos]) {
            status[pos] = false;
            --curr_size;
            add(info_film[pos]);
            pos = hash2(pos, ++i);
        }
    }

public:

    HashTab(unsigned int max_size) {

        this->max_size = max_size;
        this->curr_size = 0;
        this->info_film = new Movie[max_size];
        this->status = new bool[max_size];
        for (int i = 0; i < max_size; i++) {
            status[i] = false;
        }
    }

    ~HashTab() {

        delete[] info_film;
        delete[] status;

    }

    bool search(const Movie &film) {

        int pos = hash1(film.time_hall);
        if (film == info_film[pos]) {
            return true;
        } else {
            for (int i = 1; status[pos] && i <= max_size; ++i) {
                pos = hash2(pos, i);
                if (film == info_film[pos]) {
                    return true;
                }
            }
        }
        return false;
    }

    bool add(const Movie &film) {

        if (curr_size == max_size) {
            return false;
        }

        int pos = hash1(film.time_hall);
        if (status[pos]) {
            if (film == info_film[pos]) {
                return false;
            }
            bool added = false;
            for (int i = 1; !added && i <= max_size; ++i) {
                pos = hash2(pos, i);

                if (film == info_film[pos] && status[pos]) {
                    return false;
                }
                if (!status[pos]) {
                    info_film[pos] = film;
                    status[pos] = true;
                    curr_size++;
                    added = true;
                }
            }
        } else {
            info_film[pos] = film;
            status[pos] = true;
            curr_size++;
        }
        return true;
    }

    bool del(const Movie &film) {

        int pos = hash1(film.time_hall);
        bool deleted = false;
        int posTry = 0;

        if (film == info_film[pos]) {
            deleted = true;
        } else {
            for (int i = 1; !deleted && i <= max_size; ++i) {
                pos = hash2(pos, i);
                if (film == info_film[pos]) {
                    deleted = true;
                    posTry = i;
                }
            }
        }

        if (deleted) {
            status[pos] = false;
            --curr_size;
            ReHash(pos, posTry);
            return true;
        } else {
            return false;
        }
    }


    void print() {
        cout << "------------------------\n";

        for (int i = 0; i < max_size; ++i) {
            if (status[i]) {
                cout << "№" << i << "." << " | " << info_film[i].name << " | " << info_film[i].time_hall.number_hall
                     << " | "
                     << info_film[i].time_hall.hour << ":" << info_film[i].time_hall.minute << " | "
                     << hash1(info_film[i].time_hall) << endl;

            } else {
                cout << i << "-----------" << hash1(info_film[i].time_hall) << endl;
            }
        }
    }
};

int main() {

    HashTab MoovieList(10);
    Movie M({"QQQ", 2, 15, 30});
    for (int i = 1; i < 10; i++) {
        /*    cout <<  M.name << " | " << M.time_hall.number_hall
                       << " | "
                       << M.time_hall.hour << ":" <<M.time_hall.minute <<endl;*/
        MoovieList.add(M);
        //    M.time_hall.minute+=2;
        M.time_hall.number_hall += 1;
    }


    MoovieList.print();
    cout << endl << endl;

    M = {"QQQ", 2, 15, 30};
    for (int i = 1; i < 10; i++) {
        cout << "DEL ->" << M.name << " | " << M.time_hall.number_hall
             << " | "
             << M.time_hall.hour << ":" << M.time_hall.minute << endl;
        MoovieList.del(M);
        M.time_hall.number_hall += 2;
    }


    MoovieList.print();

    return 0;
}