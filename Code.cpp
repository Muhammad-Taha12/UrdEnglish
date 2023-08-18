#include<iostream>
#include<fstream>
#include<cstring>
#include<iomanip>
using namespace std;
//-------------Function Prototypes-------------------------------------------------------------------------------------------------------------------------------
char* readingStringFromFile();               
void fileChecker();
char* getParagraphFromUser();
char* addToWordList(char*, char*, int&, int&);
void cleanUpWordList(char*, char*, int);
void printAndOutputParagraph(char*);
void printAndOutputWordList(char*);
void removingWordsIfMultipleOccurences(char*, char*);
//----------Start of Main----------------------------------------------------------------------------------------------------------------------------------------
int main()
{
	const int number_of_characters = 3000;
	char dictionary_string[number_of_characters] = "";
	strcat(dictionary_string, readingStringFromFile());    //Copying String from file
	char user_string[number_of_characters] = "";
	strcat(user_string, getParagraphFromUser());           //Copying String taken from user
	char final_string[number_of_characters] = "";
	char word_list[number_of_characters] = "";
	char separators[3] = " \n";
	char* user_next_token;
	char* user_token = strtok_r(user_string, separators, &user_next_token);
	//----------Tokenizing the user paragraph----------------------------------------------------------------------------------------------------------------------
	while (user_token != NULL)
	{
		if (user_token != NULL)
		{
			int counter = 0;
			int urdu_counter = 1;   //Counter is used so it doesn't translate Urdu words into the English word in the next line
			int size = strlen(user_token) - 1;
			char copy_of_dictionary[number_of_characters] = "";
			strcpy(copy_of_dictionary, dictionary_string);     //Making copy of String from File
			char* input_next_token;
			char* input_token = strtok_r(copy_of_dictionary, separators, &input_next_token);
			//-------------Tokenizing the dictionary--------------------------------------------------------------------------------------------------------------
			while (input_token != NULL)
			{
				int checker = strlen(input_token);
				if (input_token != NULL)
				{
					//-------------Comparing the two tokens-------------------------------------------------------------------------------------------------------

					if (!strcmp(user_token, input_token))
					{
						if (urdu_counter % 2 != 0)
						{
							input_token = strtok_r(NULL, separators, &input_next_token);    //Making next token
							strcat(final_string, addToWordList(user_token, input_token, counter, urdu_counter));
							break;      //Breaking loop if match is found
						}
					}
					//-------------Comparing the two tokens up to the size of the word in dictionary to account for dots and commas etc.--------------------------
					else if (!strncmp(user_token, input_token, checker))      //Checking upto before any dots, commas or semicolons	
					{
						if (user_token[size] < 65 || user_token[size] > 122)
						{
							if (urdu_counter % 2 != 0)
							{
								input_token = strtok_r(NULL, separators, &input_next_token);    //Making next token
								strcat(final_string, addToWordList(user_token, input_token, counter, urdu_counter));
								break;      //Breaking loop if match is found
							}
						}
					}
					//---------------Dealing with capitalization differences depending on the dictionary------------------------------------------------------------
					else if (input_token[0] >= 97 && input_token[0] <= 122 && user_token[0] >= 65 && user_token[0] <= 90)      //Checking if the first letter is capital
					{
						input_token[0] -= 32;                    //Making first letter capital
						if (!strcmp(user_token, input_token))      //Comparing the two tokens
						{
							if (urdu_counter % 2 != 0)
							{
								input_token = strtok_r(NULL, separators, &input_next_token);    //Making next token
								input_token[0] -= 32;
								strcat(final_string, addToWordList(user_token, input_token, counter, urdu_counter));
								break;      //Breaking loop if match is found
							}
						}
						else if (!strncmp(user_token, input_token, checker))      //Checking upto before any dots, commas or semicolons
						{
							if (user_token[size] < 65 || user_token[size] > 122)
							{
								if (urdu_counter % 2 != 0)
								{
									input_token = strtok_r(NULL, separators, &input_next_token);    //Making next token
									input_token[0] -= 32;
									strcat(final_string, addToWordList(user_token, input_token, counter, urdu_counter));
									break;      //Breaking loop if match is found
								}
							}
						}
				    }
					//---------------Dealing with capitalization differences depending on the dictionary------------------------------------------------------------
					else if (input_token[0] >= 65 && input_token[0] <= 90 && user_token[0] >= 97 && user_token[0] <= 122)      //Checking if the first letter is capital
					{
						input_token[0] += 32;                    //Making first letter small
						if (!strcmp(user_token, input_token))      //Comparing the two tokens
						{
							if (urdu_counter % 2 != 0)
							{
								input_token = strtok_r(NULL, separators, &input_next_token);    //Making next token
								input_token[0] += 32;
								strcat(final_string, addToWordList(user_token, input_token, counter, urdu_counter));
								break;      //Breaking loop if match is found
							}
						}
						else if (!strncmp(user_token, input_token, checker))      //Checking upto before any dots, commas or semicolons
						{
							if (user_token[size] < 65 || user_token[size] > 122)
							{
								if (urdu_counter % 2 != 0)
								{
									input_token = strtok_r(NULL, separators, &input_next_token);    //Making next token
									input_token[0] += 32;
									strcat(final_string, addToWordList(user_token, input_token, counter, urdu_counter));
									break;      //Breaking loop if match is found
								}
							}
						}
					}
					urdu_counter++;
					input_token = strtok_r(NULL, separators, &input_next_token);    //Making next token
				}
			}
			if (counter == 0)    //If match isn't found
			{
				strcat(final_string, user_token);      //Copies whatever user entered without changing anything
				strcat(final_string, " ");
			}
			else
			{
				cleanUpWordList(user_token, input_token, size);    //Capitalizes all first letters and removes any dots and commas
				strcat(word_list, user_token);                   //Copies the words translated into the word list
				strcat(word_list, " ");
				strcat(word_list, input_token);
				strcat(word_list, " ");
			}
			user_token = strtok_r(NULL, separators, &user_next_token);    //Making next token
		}
	}
	printAndOutputParagraph(final_string);
	printAndOutputWordList(word_list);
	return 0;
}
//----------End of Main----------------------------------------------------------------------------------------------------------------------------------------

//----------Opens file and stores the data inside it in a string and returns that string to main---------------------------------------------------------------
char* readingStringFromFile()
{
	ifstream dictionary("/home/muhammad/Desktop/Projects/Other/input.txt");    //Opening File
	if (!dictionary)					//Checks if file exists
	{
		fileChecker();
	}
	char function_string[3000] = "";
	while (!dictionary.eof())
	{
		char string[100] = "";
		dictionary.getline(string, 100);    //Reading String from file
		strcat(function_string, string);
		strcat(function_string, " ");
	}
	dictionary.close();                     //Closing File
	return function_string;
}

//----------Closes program if file doesn't exist----------------------------------------------------------------------------------------------------------------
void fileChecker()
{
	cout << "File does not exist or it couldn't be opened";
	exit(0);            //Ends program if file doesn't exist
}

//----------Gets string input from user-------------------------------------------------------------------------------------------------------------------------
char* getParagraphFromUser()
{
	cout << "Enter a paragraph, press # to stop: " << endl;
	char user_paragraph[3000] = "";
	cin.getline(user_paragraph, 3000, '#');     //Getting String from user
	return user_paragraph;
}

//----------Adds translated word to the word list----------------------------------------------------------------------------------------------------------------
char* addToWordList(char* user_token, char* input_token, int& counter, int& urdu_counter)
{
	int size = strlen(user_token);
	char temp_string[3000] = "";
	strcat(temp_string, input_token);
	if (user_token[size - 1] < 65 || user_token[size - 1] > 122)    //Checks if there is anything other than a letter in the last index before \0
	{
		char temp[2] = "";
		temp[0] = user_token[size - 1];
		strcat(temp_string, temp);            //Adds whatever was at the last spot into the word
	}
	strcat(temp_string, " ");
	counter++;
	urdu_counter++;
	return temp_string;
}

//----------Removes any non-letter from the word and makes it all capital so word list is neat--------------------------------------------------------------------
void cleanUpWordList(char* English, char* Urdu, int word_size)
{
	if (English[0] >= 97 && English[0] <= 122)       //Capitalizes if small
	{
		English[0] -= 32;
	}
	if (English[word_size] < 65 || English[0] > 122)     //Removes any non-letter in the last index before \0
	{
		English[word_size] = ' ';
	}
	if (Urdu[0] >= 97 && Urdu[0] <= 122)       //Capitalizes if small
	{
		Urdu[0] -= 32;
	}
}

//----------Prints the final translated string and outputs it to the output file-----------------------------------------------------------------------------------
void printAndOutputParagraph(char* last_string)
{
	ofstream output_file("/home/muhammad/Desktop/Projects/Other/output.txt", ios::out);       //Creates/Opens file
	if (!output_file)
	{
		fileChecker();                //Checks if file can be created
	}
	cout << endl << "The translated paragraph is:" << endl << last_string << endl;
	output_file << "The translated paragraph is:" << endl << last_string << endl;
	output_file.close();
}

//---------Prints the translated words and outputs it to file--------------------------------------------------------------------------------------------------------
void printAndOutputWordList(char* final_list)

{
	ofstream output_file("/home/muhammad/Desktop/Projects/Other/output.txt", ios::app);
	int final_counter = 0;           //Used to move to next line after every two words and find total number of words translated
	cout << endl << "English:" << setw(17) << "Urdu:" << endl;
	output_file << endl << "English:" << setw(17) << "Urdu:" << endl;
	char* final_next_token;
	char* final_token = strtok_r(final_list, " ", &final_next_token);
	while (final_token != NULL)
	{
		if (final_token != NULL)
		{
			if (final_counter % 2 == 0)
			{
				int spaces = strlen(final_token);
				cout << final_token << setw(25 - spaces);
				output_file << final_token << setw(25 - spaces);
			}
			else
			{
				cout << final_token << endl;
				output_file << final_token << endl;
			}
			removingWordsIfMultipleOccurences(final_token, final_next_token);   //Removes multiple occurences of words so translated words only appear once
			final_token = strtok_r(NULL, " ", &final_next_token);
			final_counter++;
		}
	}
	final_counter = final_counter / 2;        //Divides by two because two words are outputted for every one that is translated
	cout << endl << endl << "Dictionary Words Used: " << final_counter << endl << endl;
	output_file << endl << endl << "Dictionary Words Used: " << final_counter << endl << endl;
	output_file.close();
}

//---------Compares each word with the rest of the string and removes all the next occurences of that word so it only outputs once------------------------------------
void removingWordsIfMultipleOccurences(char* token_of_first_string, char* rest_of_first_string)
{
	char multiple_occurences[3000];
	char space[2] = " ";
	strcpy(multiple_occurences, rest_of_first_string);           //Making another copy of the remainder of the first string
	char new_string[3000] = "";
	char* next_multiple_token;
	char* multiple_token = strtok_r(multiple_occurences, space, &next_multiple_token);     //Tokenizing that copy

	while (multiple_token != NULL)
	{
		if (multiple_token != NULL)
		{
			if (!strcmp(multiple_token, token_of_first_string))         //Removing all occurences of a word that appears more than once
			{															//to prevent it from printing the same word multiple times
				multiple_token = strtok_r(NULL, space, &next_multiple_token);
			}
			else
			{
				strcat(new_string, multiple_token);
				strcat(new_string, space);
				multiple_token = strtok_r(NULL, space, &next_multiple_token);
			}
		}
	}
	int size = strlen(rest_of_first_string);
	for (int loop = 0; loop < size; loop++)
	{
		rest_of_first_string[loop] = new_string[loop];                 //Copying the remainder of the string after removing all the
	}																   //words that appear multiple times into the original string
}
