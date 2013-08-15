#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <vorbis/vorbisfile.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>


#define BUFFER_SIZE (4096)

int snd_load_file(char const * file, ALuint buffer){
	FILE*           oggFile;
	OggVorbis_File  oggStream;
	vorbis_info*    vorbisInfo;
	vorbis_comment* vorbisComment;
	ALenum format;

	int result;

	if(!(oggFile = fopen(file, "rb")))
		printf("Could not open Ogg file: %s\n", file);

	if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
	{
		fclose(oggFile);
		printf("Could not open Ogg stream.\n");
		return 1;	
	}

	vorbisInfo = ov_info(&oggStream, -1);
	vorbisComment = ov_comment(&oggStream, -1);

	if(vorbisInfo->channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;


	char * dyn_data = NULL;
	int  size = 0;
	int  section;
	result = 1;
	printf("Loading sound file %s\n", file);
	while(result > 0){
		char data[BUFFER_SIZE];
		result = ov_read(&oggStream, data, BUFFER_SIZE, 0, 2, 1, & section);
		if(result > 0){
			char * tmp;
			size += result;

			tmp = (char*)malloc(sizeof(char)*(size));
			if(dyn_data != NULL){
				memcpy(tmp, dyn_data, sizeof(char)*(size-result));
				free(dyn_data);
			}
			dyn_data = tmp;
			tmp += size-result;
			memcpy(tmp, data, result);

		} else if(result < 0){
			switch(result){
				case OV_HOLE:
					printf("Interruption in the data.%d\n", result);
					printf("one of: garbage between pages, loss of sync followed by recapture, or a corrupt page\n");
					break;
				case OV_EBADLINK:
					printf("an invalid stream section was supplied to libvorbisfile, or the requested link is corrupt.\n");
					break;
				case OV_EINVAL:
					printf("the initial file headers can't be read or are corrupt, or the initial open call for vf failed.\n");
					break;
			}

		} else if(size == 0){
			printf("Date not read.\n");
		}
	}


	alBufferData(buffer, format, dyn_data, size, vorbisInfo->rate);
	free(dyn_data);
	ov_clear(&oggStream);
	return 0;
}

/*
char* loadWAV(const char* fn,int& chan,int& samplerate,int& bps,int& size){
//Loading of the WAVE file

    FILE *fp = NULL;                                                            //Create FILE pointer for the WAVE file
    fp=fopen("WAVE/Sound.wav","rb");                                            //Open the WAVE file
    if (!fp) return endWithError("Failed to open file");                        //Could not open file
    
    //Variables to store info about the WAVE file (all of them is not needed for OpenAL)
    char type[4];
    DWORD size,chunkSize;
    short formatType,channels;
    DWORD sampleRate,avgBytesPerSec;
    short bytesPerSample,bitsPerSample;
    DWORD dataSize;
    
    //Check that the WAVE file is OK
    fread(type,sizeof(char),4,fp);                                              //Reads the first bytes in the file
    if(type[0]!='R' || type[1]!='I' || type[2]!='F' || type[3]!='F')            //Should be "RIFF"
    return endWithError ("No RIFF");                                            //Not RIFF

    fread(&size, sizeof(DWORD),1,fp);                                           //Continue to read the file
    fread(type, sizeof(char),4,fp);                                             //Continue to read the file
    if (type[0]!='W' || type[1]!='A' || type[2]!='V' || type[3]!='E')           //This part should be "WAVE"
    return endWithError("not WAVE");                                            //Not WAVE
    
    fread(type,sizeof(char),4,fp);                                              //Continue to read the file
    if (type[0]!='f' || type[1]!='m' || type[2]!='t' || type[3]!=' ')           //This part should be "fmt "
    return endWithError("not fmt ");                                            //Not fmt 
    
    //Now we know that the file is a acceptable WAVE file
    //Info about the WAVE data is now read and stored
    fread(&chunkSize,sizeof(DWORD),1,fp);
    fread(&formatType,sizeof(short),1,fp);
    fread(&channels,sizeof(short),1,fp);
    fread(&sampleRate,sizeof(DWORD),1,fp);
    fread(&avgBytesPerSec,sizeof(DWORD),1,fp);
    fread(&bytesPerSample,sizeof(short),1,fp);
    fread(&bitsPerSample,sizeof(short),1,fp);
    
    fread(type,sizeof(char),4,fp);
    if (type[0]!='d' || type[1]!='a' || type[2]!='t' || type[3]!='a')           //This part should be "data"
    return endWithError("Missing DATA");                                        //not data
    
    fread(&dataSize,sizeof(DWORD),1,fp);                                        //The size of the sound data is read
    
    //Display the info about the WAVE file
    cout << "Chunk Size: " << chunkSize << "\n";
    cout << "Format Type: " << formatType << "\n";
    cout << "Channels: " << channels << "\n";
    cout << "Sample Rate: " << sampleRate << "\n";
    cout << "Average Bytes Per Second: " << avgBytesPerSec << "\n";
    cout << "Bytes Per Sample: " << bytesPerSample << "\n";
    cout << "Bits Per Sample: " << bitsPerSample << "\n";
    cout << "Data Size: " << dataSize << "\n";
    unsigned char* buf= new unsigned char[dataSize];                            //Allocate memory for the sound data
    cout << fread(buf,sizeof(BYTE),dataSize,fp) << " bytes loaded\n";           //Read the sound data and display the 

                                                                                //number of bytes loaded.

                                                                                //Should be the same as the Data Size if OK

    

}
*/

bool isBigEndian(){
        int a=1;
        return !((char*)&a)[0];
}

int convertToInt(char* buffer,int len){
        int a=0;
        if(!isBigEndian())
                for(int i=0;i<len;i++)
                        ((char*)&a)[i]=buffer[i];
        else
                for(int i=0;i<len;i++)
                        ((char*)&a)[3-i]=buffer[i];    
        return a;
}



char* loadWAV(const char* fn,int& chan,int& samplerate,int& bps,int& size){
        char buffer[4];
        std::ifstream in(fn,std::ios::binary);
        in.read(buffer,4);
        if(strncmp(buffer,"RIFF",4)!=0)
        {
                std::cout << "this is not a valid WAVE file"  << std::endl;
                return NULL;
        }
        in.read(buffer,4);
        in.read(buffer,4);      //WAVE
        in.read(buffer,4);      //fmt
        in.read(buffer,4);      //16
        in.read(buffer,2);      //1
        in.read(buffer,2);
        chan=convertToInt(buffer,2);
        in.read(buffer,4);
        samplerate=convertToInt(buffer,4);
        in.read(buffer,4);
        in.read(buffer,2);
        in.read(buffer,2);
        bps=convertToInt(buffer,2);
        in.read(buffer,4);      //data
        in.read(buffer,4);
        size=convertToInt(buffer,4);
        char* data=new char[size];
        in.read(data,size);
		
	//Display the info about the WAVE file
	    //cout << "Chunk Size: " << chunkSize << "\n";
	    printf ("Format Type: %c \n", fn);
	    printf ("Channels: %d \n", chan);
	    printf ("Sample Rate: %d \n", samplerate);
	    printf ("Average Bytes Per Second: %d \n", bps);
	    //cout << "Bytes Per Sample: " << bytesPerSample << "\n";
	    //cout << "Bits Per Sample: " << bitsPerSample 
		
		
		
		
        return data;   
}
