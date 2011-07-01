#include <stdio.h>
#include <stdlib.h>

void DieWithUserMessage(const char *msg, const char *detail) {

	fputs(msg, stderr);
	fputs(": ", stderr);
	fputs(detail, stderr);
	fputs("\n", stderr);
	exit(1);
} // void DieWithUserMessage(const char *msg, const char *detail) {

void DieWithSystemMessage(const char *msg) {

	perror(msg);
	exit(1);
} // void DieWithUserMessage(const char *msg, const char *detail) {
