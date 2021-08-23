#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <windows.h>

using namespace std;
unsigned long long int PC2(unsigned int l, unsigned int r);
unsigned long long int PR(unsigned int a);
unsigned int S_result(unsigned long long int a);
void encrypt(int i);
void decrypt(int i);
char dataToDES[8];
unsigned int key[192];
int main() {

    //===============================Pobranie kluczy z pliku key.txt===================================

    FILE *pt = fopen("key.txt", "rb");
    int z = 0, ch;
    while (!feof(pt))
    {
        ch = getc(pt);
        key[z++] = ch - 48;
    }
    fclose(pt);


    //=================================Pobranie odpowiednich danych jakie mają zostać zaszyfrowane z pliku input.txt oraz podzielone na bloki 8 znakowe=====================================

    FILE *inp = fopen("input.txt", "rb");
    FILE *out = fopen("output.txt", "wb+");
    long int size;
    if (fseek(inp, 0L, SEEK_END))
        perror("fseek() failed");
    else // size will contain no. of chars in input file.
        size = ftell(inp);
    fclose(inp);
    //system( "cls" );
    FILE *in = fopen("input.txt", "rb");
    for(int counter=0;counter<size/8+1;counter++) {
        for (int k = 0; k <= 7; ++k) {
            dataToDES[k] = fgetc(inp);
            cout << dataToDES[k];
        }
        cout << "   ";

        cout << endl;
        encrypt(0);
        decrypt(1);
        encrypt(2);
        for (int i = 0; i <= 7; i++) {
            cout<<dataToDES[i];
            out = fopen("output.txt", "ab+");
            fprintf(out, "%c", dataToDES[i]);
        }
        cout<<endl<<endl;
    }

    return 0;
}
void decrypt(int etap){
    __int64 dataDES64bits = 0;
    int power = 63;
    for (int i = 0; i <= 7; i++) {
        for (int j = 7; j >= 0; j--) {
            if ((dataToDES[i] >> j) & 1 == 1) {
                dataDES64bits += pow(2, power);
            }
            power--;
        }
    }
    __int64 keyDES = 0;
    int power2 = 63;
    for (int i = 0; i <= 16; i++) {
        for (int j = 3; j >= 0; j--) {
            if (((key[etap * 64 + i] >> j) & 1) == 1) {
                keyDES += pow(2, power2);
            }
            power2--;
        }
    }
    cout << "KLUCZ: " << keyDES << endl;
    //==========================================Permutacja wstepna======================================================

    int tmp_bits[64];

    int index1 = 57;
    for (int i = 0; i <= 7; i++) {
        tmp_bits[index1] = (dataDES64bits >> i) & 1;
        index1 -= 8;
    }

    int index2 = 59;
    for (int i = 8; i <= 15; i++) {
        tmp_bits[index2] = (dataDES64bits >> i) & 1;
        index2 -= 8;
    }

    int index3 = 61;
    for (int i = 16; i <= 23; i++) {
        tmp_bits[index3] = (dataDES64bits >> i) & 1;
        index3 -= 8;
    }
    int index4 = 63;
    for (int i = 24; i <= 31; i++) {
        tmp_bits[index4] = (dataDES64bits >> i) & 1;
        index4 -= 8;
    }
    int index5 = 56;
    for (int i = 32; i <= 39; i++) {
        tmp_bits[index5] = (dataDES64bits >> i) & 1;
        index5 -= 8;
    }
    int index6 = 58;
    for (int i = 40; i <= 47; i++) {
        tmp_bits[index6] = (dataDES64bits >> i) & 1;
        index6 -= 8;
    }
    int index7 = 60;
    for (int i = 48; i <= 55; i++) {
        tmp_bits[index7] = (dataDES64bits >> i) & 1;
        index7 -= 8;
    }
    int index8 = 62;
    for (int i = 56; i <= 63; i++) {
        tmp_bits[index8] = (dataDES64bits >> i) & 1;
        index8 -= 8;
    }

    __int64 dataDES64bitsPerm = 0;
    unsigned int left = 0;
    unsigned int right = 0;

    int power3 = 63;
    int power4 = 32;
    int power5 = 32;

    for (int i = 0; i <= 63; i++) {
        if (tmp_bits[i] == 1) {
            dataDES64bitsPerm += pow(2, power3);
        }
        power3--;
    }

    for (int i = 0; i <= 31; i++) {
        if (tmp_bits[i] == 1) {
            left += pow(2, power4);
        }
        power4--;
    }
    for (int i = 32; i <= 63; i++) {
        if (tmp_bits[i] == 1) {
            right += pow(2, power5);
        }
        power5--;
    }

    //=============================56bitowy klucz================================================================
    int tmp_key_bits[56];
    int bit_eight = 1;
    for (int i = 0; i <= 55; i++) {

        if (bit_eight != 8) {
            tmp_key_bits[i] = (keyDES >> i) & 1;
            bit_eight++;
        } else {
            bit_eight = 1;
        }
    }

    unsigned long long int keyDES56bit = 0;

    int power6 = 55;

    for (int i = 0; i <= 55; i++) {
        if (tmp_key_bits[i] == 1) {
            keyDES56bit += pow(2, power6);
        }
        power6--;
    }
    //cout<<keyDES56bit;
    //===================================permutacja klucza ====================================================

    __int64 keyDES56bitPerm = 0;
    int tmp_bits_key[64];

    int index1a = 56;
    for (int i = 0; i <= 7; i++) {
        tmp_bits_key[index1a] = (keyDES56bit >> i) & 1;
        index1a -= 8;
    }

    int index2a = 57;
    for (int i = 8; i <= 15; i++) {
        tmp_bits_key[index2a] = (keyDES56bit >> i) & 1;
        index2a -= 8;
    }

    int index3a = 58;
    for (int i = 16; i <= 23; i++) {
        tmp_bits_key[index3a] = (keyDES56bit >> i) & 1;
        index3a -= 8;
    }
    int index4a = 59;
    for (int i = 24; i <= 37; i++) {
        tmp_bits_key[index4a] = (keyDES56bit >> i) & 1;
        index4a -= 8;
    }
    int index5a = 62;
    for (int i = 28; i <= 35; i++) {
        tmp_bits_key[index5a] = (keyDES56bit >> i) & 1;
        index5a -= 8;
    }
    int index6a = 61;
    for (int i = 36; i <= 43; i++) {
        tmp_bits_key[index6a] = (keyDES56bit >> i) & 1;
        index6a -= 8;
    }
    int index7a = 60;
    for (int i = 44; i <= 51; i++) {
        tmp_bits_key[index7a] = (keyDES56bit >> i) & 1;
        index7a -= 8;
    }
    int index8a = 27;
    for (int i = 52; i <= 55; i++) {
        tmp_bits_key[index8a] = (keyDES56bit >> i) & 1;
        index8a -= 8;
    }

    power6 = 55;
    for (int i = 0; i <= 55; i++) {
        if (tmp_bits_key[i] == 1) {
            keyDES56bitPerm += pow(2, power6);
        }
        power6--;
    }

    unsigned int leftKey = 0;
    unsigned int rightKey = 0;
    int power7 = 27;
    int power8 = 27;

    for (int i = 0; i <= 26; i++) {
        if (tmp_bits_key[i] == 1) {
            leftKey += pow(2, power7);
        }
        power7--;
    }
    for (int i = 26; i <= 55; i++) {
        if (tmp_bits_key[i] == 1) {
            rightKey += pow(2, power8);
        }
        power8--;
    }

    //==========================wyliczanie 16 podkluczy ============================================================

    int offset[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    unsigned long long int K[16];
    for (int i = 0; i <= 15; i++) {
        leftKey = leftKey << offset[i];
        rightKey = rightKey << offset[i];

        K[i] = PC2(leftKey, rightKey);
    }
/*
    for(int i = 0; i<=15 ; i++){
        cout<<K[i]<<endl;
    }
*/
//===============================16 iteracji funckji f ============================================================
    unsigned long long int newRight;
    unsigned long long int newLeft;
    unsigned long long int PRRight;
    unsigned long long int rightXorKey;

    unsigned long long int tmpleft;


    newLeft = left;
    newRight = right;

    for (int i = 0; i <= 15; i++) {
        //newLeft = newRight;
        PRRight = PR(newRight);
        rightXorKey = PRRight ^ K[i];
        newRight = newLeft ^ S_result(rightXorKey);

        tmpleft = newLeft;
        newLeft = newRight;
    }

//===============================lewa+prawa i permutacja koncowa ================================================


    int tmp_bits_left_plus_right[64];
    int tmp_bits_left_plus_right_perm[64];

    int offset_left = 0;
    for (int i = 0; i <= 31; i++) {
        tmp_bits_left_plus_right[i] = (tmpleft >> offset_left) & 1;
        offset_left++;
    }
    int offset_right = 0;
    for (int i = 32; i <= 63; i++) {
        tmp_bits_left_plus_right[i] = (newRight >> offset_right) & 1;
        offset_right++;
    }


    int last_perm[64] = {
            40, 8, 48, 16, 56, 24, 64, 32,
            39, 7, 47, 15, 55, 23, 63, 31,
            38, 6, 46, 14, 54, 22, 62, 30,
            37, 5, 45, 13, 53, 21, 61, 29,
            36, 4, 44, 12, 52, 20, 60, 28,
            35, 3, 43, 11, 51, 19, 59, 27,
            34, 2, 42, 10, 50, 18, 58, 26,
            33, 1, 41, 9, 49, 17, 57, 25
    };

    for (int i = 0; i <= 63; i++) {
        tmp_bits_left_plus_right_perm[i] = tmp_bits_left_plus_right[last_perm[i] - 1];
    }

    unsigned long long int ResultDES = 0;
    power = 63;
    for (int i = 0; i <= 63; i++) {
        if (tmp_bits_left_plus_right_perm[i] == 1) {
            ResultDES += pow(2, power);
        }
        power--;
    }

    for (int i = 0; i <= 31; i++) {
        tmp_bits_left_plus_right[i] = (tmpleft >> offset_left) & 1;
        offset_left++;
    }
//=========================================WYNIK=====================================================================
    char result[8];
    int counter = 7;
    int tmp_char = 0;
    int index_char = 0;
    int tmp_char_result[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i <= 63; i++) {

        if (counter == 0) {
            if (tmp_bits_left_plus_right_perm[i] == 1) {
                tmp_char += pow(2, counter);
                counter = 7;
                tmp_char_result[index_char] = tmp_char;
                index_char++;
                tmp_char = 0;
            } else {
                counter = 7;
                tmp_char_result[index_char] = tmp_char;
                index_char++;
                tmp_char = 0;
            }

        } else {
            if (tmp_bits_left_plus_right_perm[i] == 1) {
                tmp_char += pow(2, counter);
                counter--;
            } else {
                counter--;
            }
        }
    }


    for (int i = 0; i <= 7; i++) {

        dataToDES[i] = char(tmp_char_result[i]);
    }
    cout<<endl;
}
void encrypt(int etap) {
    __int64 dataDES64bits = 0;
    int power = 63;
    for (int i = 0; i <= 7; i++) {
        for (int j = 7; j >= 0; j--) {
            if ((dataToDES[i] >> j) & 1 == 1) {
                dataDES64bits += pow(2, power);
            }
            power--;
        }
    }
    __int64 keyDES = 0;
    int power2 = 63;
    for (int i = 0; i <= 16; i++) {
        for (int j = 3; j >= 0; j--) {
            if (((key[etap * 64 + i] >> j) & 1) == 1) {
                keyDES += pow(2, power2);
            }
            power2--;
        }
    }
    cout << "KLUCZ: " << keyDES << endl;
    //==========================================Permutacja wstepna======================================================

    int tmp_bits[64];

    int index1 = 57;
    for (int i = 0; i <= 7; i++) {
        tmp_bits[index1] = (dataDES64bits >> i) & 1;
        index1 -= 8;
    }

    int index2 = 59;
    for (int i = 8; i <= 15; i++) {
        tmp_bits[index2] = (dataDES64bits >> i) & 1;
        index2 -= 8;
    }

    int index3 = 61;
    for (int i = 16; i <= 23; i++) {
        tmp_bits[index3] = (dataDES64bits >> i) & 1;
        index3 -= 8;
    }
    int index4 = 63;
    for (int i = 24; i <= 31; i++) {
        tmp_bits[index4] = (dataDES64bits >> i) & 1;
        index4 -= 8;
    }
    int index5 = 56;
    for (int i = 32; i <= 39; i++) {
        tmp_bits[index5] = (dataDES64bits >> i) & 1;
        index5 -= 8;
    }
    int index6 = 58;
    for (int i = 40; i <= 47; i++) {
        tmp_bits[index6] = (dataDES64bits >> i) & 1;
        index6 -= 8;
    }
    int index7 = 60;
    for (int i = 48; i <= 55; i++) {
        tmp_bits[index7] = (dataDES64bits >> i) & 1;
        index7 -= 8;
    }
    int index8 = 62;
    for (int i = 56; i <= 63; i++) {
        tmp_bits[index8] = (dataDES64bits >> i) & 1;
        index8 -= 8;
    }


    unsigned int left = 0;
    unsigned int right = 0;

    int power4 = 32;
    int power5 = 32;



    for (int i = 0; i <= 31; i++) {
        if (tmp_bits[i] == 1) {
            left += pow(2, power4);
        }
        power4--;
    }
    for (int i = 32; i <= 63; i++) {
        if (tmp_bits[i] == 1) {
            right += pow(2, power5);
        }
        power5--;
    }

    //=============================56bitowy klucz================================================================
    int tmp_key_bits[56];
    int bit_eight = 1;
    for (int i = 0; i <= 55; i++) {

        if (bit_eight != 8) {
            tmp_key_bits[i] = (keyDES >> i) & 1;
            bit_eight++;
        } else {
            bit_eight = 1;
        }
    }

    unsigned long long int keyDES56bit = 0;

    int power6 = 55;

    for (int i = 0; i <= 55; i++) {
        if (tmp_key_bits[i] == 1) {
            keyDES56bit += pow(2, power6);
        }
        power6--;
    }
    //cout<<keyDES56bit;
    //===================================permutacja klucza ====================================================

    __int64 keyDES56bitPerm = 0;
    int tmp_bits_key[64];

    int index1a = 56;
    for (int i = 0; i <= 7; i++) {
        tmp_bits_key[index1a] = (keyDES56bit >> i) & 1;
        index1a -= 8;
    }

    int index2a = 57;
    for (int i = 8; i <= 15; i++) {
        tmp_bits_key[index2a] = (keyDES56bit >> i) & 1;
        index2a -= 8;
    }

    int index3a = 58;
    for (int i = 16; i <= 23; i++) {
        tmp_bits_key[index3a] = (keyDES56bit >> i) & 1;
        index3a -= 8;
    }
    int index4a = 59;
    for (int i = 24; i <= 37; i++) {
        tmp_bits_key[index4a] = (keyDES56bit >> i) & 1;
        index4a -= 8;
    }
    int index5a = 62;
    for (int i = 28; i <= 35; i++) {
        tmp_bits_key[index5a] = (keyDES56bit >> i) & 1;
        index5a -= 8;
    }
    int index6a = 61;
    for (int i = 36; i <= 43; i++) {
        tmp_bits_key[index6a] = (keyDES56bit >> i) & 1;
        index6a -= 8;
    }
    int index7a = 60;
    for (int i = 44; i <= 51; i++) {
        tmp_bits_key[index7a] = (keyDES56bit >> i) & 1;
        index7a -= 8;
    }
    int index8a = 27;
    for (int i = 52; i <= 55; i++) {
        tmp_bits_key[index8a] = (keyDES56bit >> i) & 1;
        index8a -= 8;
    }

    power6 = 55;
    for (int i = 0; i <= 55; i++) {
        if (tmp_bits_key[i] == 1) {
            keyDES56bitPerm += pow(2, power6);
        }
        power6--;
    }

    unsigned int leftKey = 0;
    unsigned int rightKey = 0;
    int power7 = 27;
    int power8 = 27;

    for (int i = 0; i <= 26; i++) {
        if (tmp_bits_key[i] == 1) {
            leftKey += pow(2, power7);
        }
        power7--;
    }
    for (int i = 26; i <= 55; i++) {
        if (tmp_bits_key[i] == 1) {
            rightKey += pow(2, power8);
        }
        power8--;
    }

    //==========================wyliczanie 16 podkluczy ============================================================

    int offset[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    unsigned long long int K[16];
    for (int i = 0; i <= 15; i++) {
        leftKey = leftKey << offset[i];
        rightKey = rightKey << offset[i];

        K[i] = PC2(leftKey, rightKey);
    }
/*
    for(int i = 0; i<=15 ; i++){
        cout<<K[i]<<endl;
    }
*/
//===============================16 iteracji funckji f ============================================================
    unsigned long long int newRight;
    unsigned long long int newLeft;
    unsigned long long int PRRight;
    unsigned long long int rightXorKey;

    unsigned long long int tmpleft;


    newLeft = left;
    newRight = right;

    for (int i = 0; i <= 15; i++) {
        //newLeft = newRight;
        PRRight = PR(newRight);
        rightXorKey = PRRight ^ K[i];
        newRight = newLeft ^ S_result(rightXorKey);

        tmpleft = newLeft;
        newLeft = newRight;
    }

//===============================lewa+prawa i permutacja koncowa ================================================


    int tmp_bits_left_plus_right[64];
    int tmp_bits_left_plus_right_perm[64];

    int offset_left = 0;
    for (int i = 0; i <= 31; i++) {
        tmp_bits_left_plus_right[i] = (tmpleft >> offset_left) & 1;
        offset_left++;
    }
    int offset_right = 0;
    for (int i = 32; i <= 63; i++) {
        tmp_bits_left_plus_right[i] = (newRight >> offset_right) & 1;
        offset_right++;
    }


    int last_perm[64] = {
            40, 8, 48, 16, 56, 24, 64, 32,
            39, 7, 47, 15, 55, 23, 63, 31,
            38, 6, 46, 14, 54, 22, 62, 30,
            37, 5, 45, 13, 53, 21, 61, 29,
            36, 4, 44, 12, 52, 20, 60, 28,
            35, 3, 43, 11, 51, 19, 59, 27,
            34, 2, 42, 10, 50, 18, 58, 26,
            33, 1, 41, 9, 49, 17, 57, 25
    };

    for (int i = 0; i <= 63; i++) {
        tmp_bits_left_plus_right_perm[i] = tmp_bits_left_plus_right[last_perm[i] - 1];
    }

    unsigned long long int ResultDES = 0;
    power = 63;
    for (int i = 0; i <= 63; i++) {
        if (tmp_bits_left_plus_right_perm[i] == 1) {
            ResultDES += pow(2, power);
        }
        power--;
    }

    for (int i = 0; i <= 31; i++) {
        tmp_bits_left_plus_right[i] = (tmpleft >> offset_left) & 1;
        offset_left++;
    }
//=========================================WYNIK=====================================================================
    char result[8];
    int counter = 7;
    int tmp_char = 0;
    int index_char = 0;
    int tmp_char_result[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i <= 63; i++) {

        if (counter == 0) {
            if (tmp_bits_left_plus_right_perm[i] == 1) {
                tmp_char += pow(2, counter);
                counter = 7;
                tmp_char_result[index_char] = tmp_char;
                index_char++;
                tmp_char = 0;
            } else {
                counter = 7;
                tmp_char_result[index_char] = tmp_char;
                index_char++;
                tmp_char = 0;
            }

        } else {
            if (tmp_bits_left_plus_right_perm[i] == 1) {
                tmp_char += pow(2, counter);
                counter--;
            } else {
                counter--;
            }
        }
    }


    for (int i = 0; i <= 7; i++) {

        dataToDES[i] = char(tmp_char_result[i]);
    }
    cout<<endl;
}









// ============================funkcje permutacji ====================================================





unsigned long long int PC2(unsigned int l, unsigned int r){
    int power = 47;
    unsigned long long int K = 0;
    int table_perm_keys[] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
                             23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
                             41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
                             44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

    int new_key48[48];
    int old_key56[56];
    int offset=0;
    for(int i = 0; i <= 27; i++){
        old_key56[i] = (l  >> offset) &1;
        offset++;
    }

    for(int i = 28; i <= 55; i++){
        old_key56[i] = (r  >> offset) &1;
        offset++;
    }
//kompresja
    for(int i=0; i<=47 ; i++){
        new_key48[i] = old_key56[table_perm_keys[i] -1];
    }


    for(int i =0; i <=47 ; i++){
        if(new_key48[i] == 1) {
            K += pow(2, power);
        }
        power--;
    }

    return K;
}

unsigned long long int PR(unsigned int a){
    int table_perm_keys[] ={ 32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
                             8, 9, 10, 11, 12, 12, 12, 13, 14, 15, 16, 17,
                             16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
                             24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};

    int old_key32[32];
    int new_key48[48];
    int offset=0;
    int power=47;
    for(int i = 0; i <= 31; i++){
        old_key32[i] = (a  >> offset) &1;
        offset++;
    }

    //rozszerzenie
    for(int i=0; i<=47 ; i++){
        new_key48[i] = old_key32[table_perm_keys[i] -1];
    }
    unsigned long long int A = 0;
    for(int i =0; i <=47 ; i++){
        if(new_key48[i] == 1) {
            A+= pow(2, power);
        }
        power--;
    }

    return A;

}

unsigned int S_result(unsigned long long int a){
    unsigned int Result;

    int S1[4][16] = {
            14,	4,	13,	1,	2,	15,	11,	8,	3,	10,	6,	12,	5,	9,	0,	7,
            0,	15,	7,	4,	14,	2	,13,	1,	10,	6,	12,	11,	9,	5,	3,	8,
            4,	1,	14,	8,	13,	6,	2,	11,	15,	12,	9, 7,	3,	10,	5,	0,
            15,	12,	8	,2,	4,	9,	1,	7,	5,	11,	3,	14,	10,	0,	6,	13
    };
    int S2[4][16] = {
            15,	1,	8,	14,	6,	11,	3,	4,	9,	7,	2,	13,	12,	0,	5,	10,
            3,	13,	4,	7,	15,	2,	8,	14,	12,	0,	1,	10,	6,	9,	11,	5,
            0,	14,	7,	11,	10,	4,	13,	1,	5,	8,	12,	6,	9,	3,	2,	15,
            13,	8,	10	,1,	3,	15,	4,	2,	11,	6,	7,	12,	0,	5,	14,	9
    };
    int S3[4][16] = {
            10,	0,	9,	14,	6,	3,	15	,5,	1,	13,	12,	7,	11,	4,	2,	8,
            13,	7,	0,	9,	3,	4	,6,	10,	2,	8,	5,	14,	12,	11,	15,	1,
            13,	6,	4,	9,	8,	15,	3	,0,	1,	1,	2,	12,	5,	10,	14,	7,
            1,	10,	13,	0,	6,	9,	8	,7,	4,	15,	14,	3,	11,	5,	2,	12

    };
    int S4[4][16] = {
            7,	13,	14,	3,	0	,6,	9,	10,	1,	2,	8,	5,	11,	12,	4,	15,
            13,	8,	11,	5	,6,	15,	0,	3,	4,	7,	2,	12,	1,	10,	14,	9,
            10,	6,	9	,0,	12,	11,	7,	13,	15,	1,	3,	14,	5,	2,	8,	4,
            3,  15,	0,	6,  10, 1,	13,	8,	9,	4,	5,	11,	12,	7,	2,	14
    };
    int S5[4][16] = {
            2 ,   12,    4,    1,    7 ,   10,    11 ,   6 ,   8,    5  ,  3,    15   , 13 ,   0 ,   14,    9,
            14 ,   11,    2 ,   12,    4 ,   7  ,  13 ,   1 ,   5  ,  0  ,  15   , 10 ,   3 ,   9  ,  8  ,  6,
            4 ,   2 ,   1   , 11 ,   10  ,  13   , 7   , 8   , 15 ,   9   , 12  ,  5  ,  6 ,   3   , 0  ,  14,
            11 ,   8,    12  ,  7,    1   , 14 ,   2 ,   13   , 6,    15   , 0 ,   9  ,  10,    4   , 5,    3
    };
    int S6[4][16] = {
            12,	1,	10,	15,	9	,2,	6,  8,	0,	13,	3,	4,	14,	7,	5,	11,
            10,	15,	4,	2	,7,	12,	9,	5,	6,	1,	13,	14,	0,	11,	3,	8,
            9	,14,	15,	5	,2,	8,	12,	3,	7,	0,	4,	10,	1,	13,	11,	6,
            4	,3,	2,	12,	9	,5,	15,	10,	11,	14,	1,	7,	6,	0,	8,	13
    };
    int S7[4][16] = {
            4,	11,	2,	14,	15,	0,	8,	13,	3,	12,	9,	7,	5,	10,	6,	1,
            13,	0,	11,	7,	4,	9,	1,	10,	14,	3,	5,	12,	2,	15,	8,	6,
            1,	4,	11,	13,	12,	3,	7,	14,	10,	15,	6,	8,	0,	5,	9,	2,
            6,	11,	13,	8, 1,	4,	10,	7	,9,	5,	0,	15,	14,	2,	3,	12
    };
    int S8[4][16] = {
            13,	2,	8,	4,	6,	15,	11,	1,	10,	9,	3,	14,	5,	0,	12,	7,
            1,	15,	13,	8,	10,	3,	7,	4,	12,	5,	6,	11,	0,	14,	9,	2,
            7,	11,	4,	1,	9,	12,	14,	2,	0,	6,	10,	13,	15,	3,	5,	8,
            2,	1,	14,	7,	4,	10,	8,	13,	15,	12,	9,	0,	3, 5,	6,	11
    };

    int bitsFromA[48];
    int bitsResult[32];
    int offset = 0;
    for(int i=0; i<=47; i++){
        bitsFromA[i] = (a  >> offset) &1;
        offset++;
        //   cout<<bitsFromA[i];
    }

    int tmp_result[8];

    //0 i 5
    int y = bitsFromA[0]*2+bitsFromA[5];
    int x = bitsFromA[1]*8+bitsFromA[2]*4+bitsFromA[3]*2+bitsFromA[4];
    tmp_result[0] = S1[bitsFromA[y]][x];

    y = bitsFromA[6]*2+bitsFromA[11];
    x = bitsFromA[7]*8+bitsFromA[8]*4+bitsFromA[9]*2+bitsFromA[10];
    tmp_result[1] = S1[bitsFromA[y]][x];

    y = bitsFromA[12]*2+bitsFromA[17];
    x = bitsFromA[13]*8+bitsFromA[14]*4+bitsFromA[15]*2+bitsFromA[16];
    tmp_result[2] = S1[bitsFromA[y]][x];

    y = bitsFromA[18]*2+bitsFromA[23];
    x = bitsFromA[19]*8+bitsFromA[20]*4+bitsFromA[21]*2+bitsFromA[22];
    tmp_result[3] = S1[bitsFromA[y]][x];

    y = bitsFromA[24]*2+bitsFromA[29];
    x = bitsFromA[25]*8+bitsFromA[26]*4+bitsFromA[27]*2+bitsFromA[28];
    tmp_result[4] = S1[bitsFromA[y]][x];

    y = bitsFromA[30]*2+bitsFromA[35];
    x = bitsFromA[31]*8+bitsFromA[32]*4+bitsFromA[33]*2+bitsFromA[34];
    tmp_result[5] = S1[bitsFromA[y]][x];

    y = bitsFromA[36]*2+bitsFromA[41];
    x = bitsFromA[37]*8+bitsFromA[38]*4+bitsFromA[39]*2+bitsFromA[40];
    tmp_result[6] = S1[bitsFromA[y]][x];

    y = bitsFromA[42]*2+bitsFromA[47];
    x = bitsFromA[43]*8+bitsFromA[44]*4+bitsFromA[45]*2+bitsFromA[46];
    tmp_result[7] = S1[bitsFromA[y]][x];

    /* for(int i=0; i<=7; i++){
        // cout<<endl;
        // cout<<tmp_result[i];
     }
     */

    int index=0;
    for(int k=0; k<=7; k++) {
        for (int i = 3; i >= 0; i--) {
            if(((tmp_result[k]>>i)&1)==1){
                bitsResult[index] = 1;
                index++;
            }
            else{
                bitsResult[index] = 0;
                index++;
            }
        }
    }




    //permutacja wyniku z s-blokow

    int newBitsResult[32];
    int perm[32] = {
            16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
            2, 8 ,24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
    };

    for(int i=0; i<=31; i++){
        newBitsResult[i] = bitsResult[perm[i] - 1];
    }


    int power = 31;
    for(int i =0; i <=31 ; i++){
        if(newBitsResult[i] == 1) {
            Result+= pow(2, power);
        }
        power--;
    }
    /*
    cout<<endl;
    cout << Result;
     */

    return Result;
}
