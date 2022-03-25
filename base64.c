//=================================================_
//   Base64 Encoding and Decoding Tool             |_ 
//      Author: SynActktraa [Mikey]                 |_
// (Cli Wizard) base64 algorithm implemented in C.   |_
//     © This tool is based on ASCII charset.         |_
//=======================================================


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"base64_utils.h"


void encode(char*string){

    int string_len = Strlen(string);

    int base64_val_space = (0.4*(string_len+1))+string_len+2;
    int bin_dump_space = (string_len * 8)+1;

    char six_bit_bin[7], Ox49_val_bin[9]; 

    char *base64_val = (char*)malloc(sizeof(char) * base64_val_space);
    char *bin_dump = (char*)malloc(sizeof(char) * bin_dump_space);

    int i, j, k, bin_dump_len;
    
    memset(Ox49_val_bin, 0, Strlen(Ox49_val_bin));
    memset(bin_dump, 0, Strlen(bin_dump));

    for(i=0; string[i] != '\0'; ++i){
        strcpy(Ox49_val_bin, decToBin(string[i]));
		while(Strlen(Ox49_val_bin)%8 != 0){

        	k = insert(Ox49_val_bin, 0, '0', Strlen(Ox49_val_bin), Strlen(Ox49_val_bin)+1);
        	Ox49_val_bin[k] = '\0';
    }   
        strcat(bin_dump, Ox49_val_bin);
        memset(Ox49_val_bin, 0, Strlen(Ox49_val_bin));

    }

    bin_dump_len = Strlen(bin_dump);
    while(bin_dump_len%6 != 0)
        bin_dump_len = insert(bin_dump, bin_dump_len, '0', bin_dump_len, Strlen(bin_dump)+1);

    i = 0, j = 0;   
    while(i[bin_dump]!='\0'){

        memset(six_bit_bin, 0, Strlen(six_bit_bin));
        memmove(six_bit_bin, bin_dump+i, 6);
        int ascii_val = binToDec(six_bit_bin);

        if(ascii_val>=0 && ascii_val<=25)
            base64_val[j] = ascii_val+65;
        else if(ascii_val>=26 && ascii_val<=51)
            base64_val[j] = ascii_val+71;
        else if(ascii_val>=52 && ascii_val<=61)
            base64_val[j] = ascii_val-4;
        else if(ascii_val==62)
            base64_val[j] = ascii_val-19;
        else if(ascii_val==63)
            base64_val[j] = ascii_val-16;
        
        j++; i += 6;

    }
    base64_val[j] = '\0';

    free(bin_dump);

    while(Strlen(base64_val)%4 != 0)
        insert(base64_val, Strlen(base64_val), 0x3d, Strlen(base64_val), base64_val_space);

    fwrite(base64_val, 1, Strlen(base64_val), stdout);
    free(base64_val); 

}

void decode(char*base64_data){

	int i, j, data_len = Strlen(base64_data);

    int decData_val_space = (data_len+2)-(0.15*data_len);
    int bin_dump_space = (data_len * 6)+1;

    char Ox49_val_bin[10], byte_bin[10];
    char *bin_dump = (char*)malloc(sizeof(char) * bin_dump_space);
    char *decodeData = (char*)malloc(sizeof(char) * decData_val_space);

	while(*(base64_data+(data_len-1)) == 0x3D){
		data_len = delete(base64_data, data_len-1, data_len);
	}

    memset(bin_dump, 0, Strlen(bin_dump));
    memset(Ox49_val_bin, 0, Strlen(Ox49_val_bin));

    for(i=0; base64_data[i]!=0; ++i){
        if(base64_data[i]>='B' && base64_data[i]<='Z'){
            strcpy(Ox49_val_bin, decToBin(base64_data[i]-65));
        } else if(base64_data[i]>='a' && base64_data[i]<='z'){
            strcpy(Ox49_val_bin, decToBin(base64_data[i]-71));
        } else if(base64_data[i]>='0' && base64_data[i]<='9'){
            strcpy(Ox49_val_bin, decToBin(base64_data[i]+4));
        } else if(base64_data[i]=='+')
            strcpy(Ox49_val_bin, decToBin(base64_data[i]+19));
        else if(base64_data[i]=='/')
            strcpy(Ox49_val_bin, decToBin(base64_data[i]+16));
        else if(base64_data[i] == 'A')
            strcpy(Ox49_val_bin, "000000");


        int k = Strlen(Ox49_val_bin);
        while(k%6 != 0)
            k = insert(Ox49_val_bin, 0, '0', k, sizeof(Ox49_val_bin));
        
        strcat(bin_dump, Ox49_val_bin);
        memset(Ox49_val_bin, 0, Strlen(Ox49_val_bin));
    }

    int bin_dump_len = Strlen(bin_dump);
    while(bin_dump_len%8 != 0)
        bin_dump_len = insert(bin_dump, bin_dump_len , '0', bin_dump_len, bin_dump_space);
    *(bin_dump+bin_dump_len) = '\0';

    i = 0, j = 0;
    while(bin_dump[i]!='\0'){

        memset(byte_bin, 0, Strlen(byte_bin));
        memmove(byte_bin, bin_dump+i, 8);
        decodeData[j] = binToDec(byte_bin);
        j++; i += 8;
    }
    decodeData[j] = '\0';

    free(bin_dump);

    if(*(decodeData) < ' ' || *(decodeData) > '~'){
        fprintf(stderr, "Error: The string to be decoded is not correctly encoded.\n");
        exit(1);

    } else {
        data_len = Strlen(decodeData);
        for(i=1; decodeData[i] != '\0'; ++i){
            if(decodeData[i]< ' ' || decodeData[i]> '~'){
                data_len = delete(decodeData, i, data_len);
            }
        }
    }

    fwrite(decodeData, 1, data_len, stdout);
    free(decodeData);

}

int main(int argc, char* argv[]){
	
    if(argc==2){
        if(!strcmp(argv[1], "-h")){
            fprintf(stdout, "\nNote: Put space separated data in quotes.\
            \nUsage: %s -e/-d <data>\n|CLI options|:-\
            \n\t-e - Encodes the data string\
            \n\t-d - Decodes the data string\n", basename(argv[0]));

        }
    }else if(argc==3){
		if(!strcmp(argv[1], "-e")){
			encode(argv[2]);
		} else if(!strcmp(argv[1], "-d")){
			decode(argv[2]);
		}
	} else{
        fprintf(stderr, "\nUsage: %s -e/-d <data>\
        \nFor more, check help section:\
        \n    %s -h\n\n", basename(argv[0]), basename(argv[0]));
    }
}


// https://dev.to/rdentato/utf-8-strings-in-c-1-3-42a4