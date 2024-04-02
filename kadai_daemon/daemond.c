#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/*
*〇追加課題1
*デーモンプログラミング
*起動時にメッセージを表示
*10秒後、メッセージを表示して終了
*/

#define OPEN_MAX_GUESS 1024
#define MSG_FORK_ERROR "fork error\nerror:%s(code:%d)\n"
#define MSG_GET_OPEN_MAX_ERROR "failed to get file descriptor openmax\nerror:%s(code:%d)\n"
#define RETURN_NORMAL_END 0
#define RETURN_FAIL_FORK 1
#define RETURN_FAIL_OPEN_MAX 2

static int get_open_max();

int main(int argc, char *argv[])
{
	pid_t pid;
	int rep_num;
	int i;
	time_t time_start;
	time_t time_stop;

	pid = fork();
	if(pid < 0) {
		fprintf(stderr, MSG_FORK_ERROR, strerror(errno), errno);
		return RETURN_FAIL_FORK;
	} else if(pid != 0) {
		exit(0);
	}

	setsid();
	chdir("/");

	/* stdout, stderr以外のfdをclose */
	rep_num = get_open_max();
	if(rep_num == -1) {
		fprintf(stderr, MSG_GET_OPEN_MAX_ERROR, strerror(errno), errno);
		return RETURN_FAIL_OPEN_MAX;
	}
	close(0);
	for(i = 3; i < rep_num; i++) {
		close(i);
	}

	/* 10秒待って終了 */
	time_start = time(NULL);
	fprintf(stdout, "start:%s\n", ctime(&time_start));
	sleep(10);
	time_stop = time(NULL);
	fprintf(stdout, "stop:%s\n", ctime(&time_stop));
	return RETURN_NORMAL_END;
}

/**
 * @brief オープン可能なファイルディスクリプタの最大数を返す
 *
 * @retval > 0  オープン可能なファイルディスクリプタの最大数
 * @retval -1   取得失敗
 */
static int get_open_max()
{
#ifdef OPEN_MAX
	return OPEN_MAX;
#else
	int openmax = 0;
	errno = 0;
	openmax = sysconf(_SC_OPEN_MAX);
	if(openmax < 0) {
		if(errno != 0) {
			return -1;
		} else {
			return OPEN_MAX_GUESS;
		}
	}
	return openmax;
#endif
}

