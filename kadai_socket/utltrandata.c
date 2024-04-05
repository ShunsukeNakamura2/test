#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

/*
* 〇課題 ソケット
* コマンドライン引数によって、受信側と送信側に分かれて、
* ソケット通信を行う
* 送信側は、指定したファイルの内容を、送信側に送る
* 受信側は、送られてきた情報を、指定したファイルに追記する
* */

#define MSG_USAGE \
	"usage:\n"\
	"utltrandata -s portno -h hostname -f filename\n"\
	"	-h hostname	指定されたホストに対して接続します。\n"\
	"	-s portno	指定されたポート番号に対して接続します。\n"\
	"	-f filename	指定されたファイルからデータを読み込みます。\n\n"\
	"utltrandata -r portno -f filename\n"\
	"	-r portno	指定されたポート番号で接続を待ちます。\n"\
	"	-f filename	指定されたファイルにデータを追記します。\n"
#define MSG_ERROR_COMMON "error:%s (code:%d)\n"
#define MSG_FILE_ERROR "file %s error\n"MSG_ERROR_COMMON
#define MSG_CLOSE_ERROR "failed to close %s\n"MSG_ERROR_COMMON
#define MSG_FORK_ERROR "fork error\n"MSG_ERROR_COMMON
#define MSG_GET_OPEN_MAX_ERROR "failed to get file descriptor openmax\n"MSG_ERROR_COMMON
#define MSG_SOCKET_ERROR "failed to socket\n"MSG_ERROR_COMMON
#define MSG_BIND_ERROR "failed to bind\n"MSG_ERROR_COMMON
#define MSG_LISTEN_ERROR "failed to listen\n"MSG_ERROR_COMMON
#define MSG_ACCEPT_ERROR "failed to accept\n"MSG_ERROR_COMMON
#define MSG_SEND_ERROR "failed to send\n"MSG_ERROR_COMMON
#define MSG_RECV_ERROR "failed to recv\n"MSG_ERROR_COMMON
#define MSG_NAME_RESOL_ERROR "failed to host name resolution\n"MSG_ERROR_COMMON
#define MSG_CONNECT_ERROR "failed to connect\n"MSG_ERROR_COMMON
#define MSG_SELECT_ERROR "failed to select\n"MSG_ERROR_COMMON
#define MSG_SEND_RETRY "failed to send > retry...\n"
#define MSG_RECV_RETRY "failed to receive > retry...\n"
#define MSG_SEND_TIMEOUT "send:socket timeout\n"
#define MSG_RECV_TIMEOUT "recv:socket timeout\n"
#define MSG_DAEMON_INIT_ERROR "failed to daemonize process\n"
#define MSG_SEND_PROCESS_ERROR "failed to send process\n"
#define MSG_RECV_PROCESS_ERROR "failed to receive process\n"
#define MSG_USE_WELL_KNOUN_PORT "cannot use well known port\n"
#define RETURN_NORMAL_END 0
#define RETURN_USAGE 1
#define RETURN_FAIL_DAEMONIZE 2
#define RETURN_FAIL_SEND 3
#define RETURN_FAIL_RECV 4
#define MODE_SEND 1
#define MODE_RECV 2
#define ARGC_MIN 5
#define OPEN_MAX_GUESS 1024
#define BUFF_SIZE 1024
#define MAX_SEND_RETRY 10
#define MAX_RECV_RETRY 10
#define TIMEOUT_SEC 3

typedef struct st_args {
	char *hostname;
	char *filename;
	unsigned short portno;
	int mode;
} st_args;

static int analyze_args(int argc, char *argv[], st_args *params);
static int get_open_max();
static int daemon_init();
static int send_process(st_args params);
static int recv_process(st_args params);
static int recv_listen(unsigned short portno);
static int recv_data(int client_fd, char *filename);
static int send_connect(char *hostname, unsigned short portno);

int main(int argc, char *argv[]) {
	int main_rc = RETURN_NORMAL_END;
	int rc;
	st_args params = {NULL, NULL, 0, -1};

	rc = analyze_args(argc, argv, &params);
	if(rc == -1) {
		printf(MSG_USAGE);
		return RETURN_USAGE;
	}

	switch(params.mode) {
	case MODE_SEND:
		rc = send_process(params);
		if(rc == -1) {
			fprintf(stderr, MSG_SEND_PROCESS_ERROR);
			return RETURN_FAIL_SEND;
		}
		break;
	case MODE_RECV:
		rc = daemon_init();
		if(rc == -1) {
			fprintf(stderr, MSG_DAEMON_INIT_ERROR);
			return RETURN_FAIL_DAEMONIZE;
		}
		rc = recv_process(params);
		if(rc == -1) {
			fprintf(stderr, MSG_RECV_PROCESS_ERROR);
			return RETURN_FAIL_RECV;
		}
		break;
	default:
		
		break;
	}
	return main_rc;
}

/**
 * @brief コマンドライン引数をチェックしパラメータを構造体st_argsに入れる
 * 
 * @param[in] argc コマンドライン引数の個数
 * @param[in] argv コマンドライン引数
 * @param[in,out] params パラメータ格納用構造体
 *
 * @retval 0   正常終了
 * @retval -1  コマンドライン引数不正
 */
static int analyze_args(int argc, char *argv[], st_args *params)
{
	char *portno_str = NULL;
	char *endptr = NULL;
	int i;
	int buf;

	if(argc < ARGC_MIN ) {
		return -1;
	}

	params->hostname = NULL;
	params->filename = NULL;
	params->mode = -1;

	/* オプションとパラメータを取得 */
	for(i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-h") == 0) {
			/* ホスト名 */
			if(params->hostname != NULL) {
				return -1;
			}
			if(++i == argc) {
				return -1;
			}
			params->hostname = argv[i];
		} else if(strcmp(argv[i], "-f") == 0) {
			/* ファイル名 */
			if(params->filename != NULL) {
				return -1;
			}
			if(++i == argc) {
				return -1;
			}
			params->filename = argv[i];
		} else if(strcmp(argv[i], "-s") == 0) {
			/* 送信モード＆ポート番号 */
			if(params->mode != -1) {
				return -1;
			}
			if(++i == argc) {
				return -1;
			}
			params->mode = MODE_SEND;
			portno_str = argv[i];
		} else if(strcmp(argv[i], "-r") == 0) {
			/* 受信モード＆ポート番号 */
			if(params->mode != -1) {
				return -1;
			}
			if(++i == argc) {
				return -1;
			}
			params->mode = MODE_RECV;
			portno_str = argv[i];
		}
	}

	/* パラメータに過不足がある場合引数不正 */
	if(params->mode == MODE_SEND) {
		if(params->hostname == NULL || params->filename == NULL || portno_str == NULL) {
			return -1;
		}
	} else if(params->mode == MODE_RECV) {
		/* 受信モードではhostnameを指定してはいけない */
		if(params->hostname != NULL || params->filename == NULL || portno_str == NULL) {
			return -1;
		}
	} else {
		return -1;
	}

	buf = (int)strtol(portno_str, &endptr, 10);
	/* 文字列 -> 数値 変換失敗 */
	if(*endptr != '\0') {
		return -1;
	}
	/* 範囲外のport番号 */
	if(buf < 0 || buf > 65535) {
		return -1;
	}
	params->portno = (unsigned short)buf;

	return 0;
}

/**
 * @brief プロセスのデーモン化を行う
 *
 * @retval 0  デーモン化成功
 * @retval -1 デーモン化失敗
 */
static int daemon_init()
{
	pid_t pid;
	int rep_num;
	int i;

	pid = fork();
	if(pid < 0) {
		fprintf(stderr, MSG_FORK_ERROR, strerror(errno), errno);
		return -1;
	} else if(pid != 0) {
		exit(0);
	}

	setsid();
	chdir("/");
	umask(0);

	/* stderr以外のfdをclose */
	rep_num = get_open_max();
	if(rep_num == -1) {
		fprintf(stderr, MSG_GET_OPEN_MAX_ERROR, strerror(errno), errno);
		return -1;
	}
	close(0); /* stdin */
	close(1); /* stdout */
	for(i = 3; i < rep_num; i++) {
		close(i);
	}
	return 0;
}

/**
 * @brief open可能なファイルディスクリプタの最大数を返す
 *
 * @retval > 0 open可能なファイルディスクリプタの最大数
 * @retval -1  取得失敗
 */
static int get_open_max()
{
#ifdef OPEN_MAX
	int openmax = OPEN_MAX
#else
	int openmax = 0;
#endif
	if(openmax > 0) {
		return openmax;
	}
	errno = 0;
	openmax = sysconf(_SC_OPEN_MAX);
	if(openmax < 0) {
		if(errno != 0) {
			return -1;
		}
		return OPEN_MAX_GUESS;
	}
	return openmax;
}

/**
 * @brief  受信モードの動作
 *	   受信したデータを、指定したファイルに追記する
 *
 * @param[in] params コマンドライン引数の解析済データ
 *
 * @retval 0  正常終了(これが返されるルートは無い)
 * @retval -1 異常終了
 */
static int recv_process(st_args params)
{
	int sock_fd = -1;
	int client_fd = -1;
	int rc;
	int is_fail;
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	/* Well Known Ports が指定されていないかチェック */
	if(0 <= params.portno && params.portno <= 1023) {
		fprintf(stderr, MSG_USE_WELL_KNOUN_PORT);
		return -1;
	}

	/* 受信用のソケットを準備 */
	sock_fd = recv_listen(params.portno);
	if(sock_fd == -1) {
		return -1;
	}

	while(1) {
		/* 接続を待つ */
		client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if(client_fd == -1) {
			fprintf(stderr, MSG_ACCEPT_ERROR, strerror(errno), errno);
			is_fail = 1;
			goto end;
		}

		/* データを受信してファイルに書き込む */
		rc = recv_data(client_fd, params.filename);
		if(rc == -1) {
			is_fail = 1;
			goto end;
		}

		/* ソケットを閉じて次の接続へ */
		rc = close(client_fd);
		if(rc == -1) {
			fprintf(stderr, MSG_CLOSE_ERROR, "client socket", strerror(errno), errno);
			is_fail = 1;
			goto end;
		}
	}

end:
	/* fdを開いていたら閉じる */
	if(sock_fd > 0) {
		rc = close(sock_fd);
		if(rc == -1) {
			fprintf(stderr, MSG_CLOSE_ERROR, "server socket", strerror(errno), errno);
		}	
	}
	if(client_fd > 0) {
		rc = close(client_fd);
		if(rc == -1) {
			fprintf(stderr, MSG_CLOSE_ERROR, "client socket", strerror(errno), errno);
		}
	}

	if(is_fail) {
		return -1;
	}
	return 0;
}

/**
 * @bfief 受信側のソケットを準備し、ファイルディスクリプタを返す
 *
 * @param[in] portno    使用するポート番号
 * 
 * @retval > 0 作成したソケット
 * @retval -1  ソケット作成失敗
 */
static int recv_listen(unsigned short portno)
{
	int sock_fd;
	int rc;
	int is_fail = 0;
	struct sockaddr_in recv_addr;

	/* ソケット作成(IP V4, TCP, プロトコルは自動選択) */
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd == -1) {
		fprintf(stderr, MSG_SOCKET_ERROR, strerror(errno), errno);
		return -1;
	}

	/* アドレスの設定 */
	memset(&recv_addr, 0, sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;		/* IP V4 */
	recv_addr.sin_port = htons(portno);	/* コマンドラインで指定したポート番号 */
	recv_addr.sin_addr.s_addr = INADDR_ANY;	/* 任意のアドレスからの接続を受け付ける */

	/* バインド */
	rc = bind(sock_fd, (struct sockaddr *)&recv_addr, sizeof(recv_addr));
	if(rc == -1) {
		fprintf(stderr, MSG_BIND_ERROR, strerror(errno), errno);
		is_fail = 1;
		goto end;
	}

	/* ソケットを接続待ちに設定 接続要求を最大5件まで待たせる */
	rc = listen(sock_fd, 5);
	if(rc == -1) {
		fprintf(stderr, MSG_LISTEN_ERROR, strerror(errno), errno);
		is_fail = 1;
		goto end;
	}

end:
	/* 失敗時の後始末 */
	if(is_fail) {
		rc = close(sock_fd);
		if(rc == -1) {
			fprintf(stderr, MSG_CLOSE_ERROR, "server socket", strerror(errno), errno);
		}
		return -1;
	}

	return sock_fd;
}

/**
 * @brief クライアントから送られてきたデータをファイルに書き込む
 *
 * @param[in] client_fd 接続先クライアントのソケット
 * @param[in] filename  受信データ書き込み先のファイル名
 *
 * @retval 0  正常終了
 * @retval -1 異常終了
 */
static int recv_data(int client_fd, char *filename)
{
	int outfile_fd = -1;
	char buff[BUFF_SIZE];
	int main_rc = 0;
	int rc;
	int retry_count;
	ssize_t size;
	fd_set rfds; /* 読み込み可能か監視するfdの集合 */
	struct timeval timeout;

	outfile_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(outfile_fd == -1) {
		fprintf(stderr, MSG_FILE_ERROR, "open", strerror(errno), errno);
		main_rc = -1;
		goto end;
	}

	/* ソケットが読み込み可能か確認 */
	FD_ZERO(&rfds);
	FD_SET(client_fd, &rfds);
	timeout.tv_sec = TIMEOUT_SEC;
	timeout.tv_usec = 0;
	rc = select(client_fd + 1, &rfds, NULL, NULL, &timeout);
	if(rc < 0) {
		fprintf(stderr, MSG_SELECT_ERROR, strerror(errno), errno);
		main_rc = -1;
		goto end;
	}
	if(rc == 0) {
		fprintf(stderr, MSG_RECV_TIMEOUT);
		main_rc = -1;
		goto end;
	}

	while(1) {
		memset(buff, 0, BUFF_SIZE);
		/* recvを最大MAX_RECV_RETRY回リトライする */
		for(retry_count = 0; retry_count < MAX_RECV_RETRY; retry_count++) {
			size = recv(client_fd, buff, BUFF_SIZE, 0);
			if(size >= 0) {
				break;
			}
			fprintf(stderr, MSG_RECV_RETRY);
		}
		if(size == -1) {
			fprintf(stderr, MSG_RECV_ERROR, strerror(errno), errno);
			main_rc = -1;
			goto end;
		}
		if(size == 0) { /* 相手が接続を終わったと判断 */
			break;
		}

		size = write(outfile_fd, buff, size);
		if(size == -1) {
			fprintf(stderr, MSG_FILE_ERROR, "write", strerror(errno), errno);
			main_rc = -1;
			goto end;
		}
	}

end:
	if(outfile_fd > 0) {
		rc= close(outfile_fd);
		if(rc == -1) {
			fprintf(stderr, MSG_FILE_ERROR, "close", strerror(errno), errno);
			main_rc = -1;
		}
	}
	return main_rc;
}

/**
 * @brief 送信モードの動作
 *        指定されたファイルの内容を、受信側へ送信する
 *
 * @param[in] params コマンドライン引数の解析済データ
 *
 * @retval 0  正常終了
 * @retval -1 異常終了
 */
static int send_process(st_args params)
{
	int sock_fd = -1;
	int infile_fd = -1;
	int rc;
	int main_rc = 0;
	int retry_count;
	ssize_t read_size;
	ssize_t send_size;
	char buff[BUFF_SIZE];
	fd_set wfds; /* 書き込み可能か監視するfdの集合 */
	struct timeval timeout;

	/* 送信用のソケットを準備 */
	sock_fd = send_connect(params.hostname, params.portno);
	if(sock_fd == -1) {
		main_rc = -1;
		goto end;
	}
	
	infile_fd = open(params.filename, O_RDONLY);
	if(infile_fd == -1) {
		fprintf(stderr, MSG_FILE_ERROR, "open", strerror(errno), errno);
		main_rc = -1;
		goto end;
	}

	/* ソケットが書き込み可能か確認 */
	FD_ZERO(&wfds);
	FD_SET(sock_fd, &wfds);
	timeout.tv_sec = TIMEOUT_SEC;
	timeout.tv_usec = 0;
	rc = select(sock_fd + 1, NULL, &wfds, NULL, &timeout);
	if(rc < 0) {
		fprintf(stderr, MSG_SELECT_ERROR, strerror(errno), errno);
		main_rc = -1;
		goto end;
	}
	if(rc == 0) {
		fprintf(stderr, MSG_SEND_TIMEOUT);
		main_rc = -1;
		goto end;
	}

	while(1) {
		read_size = read(infile_fd, buff, BUFF_SIZE - 1);
		if(read_size == -1) {
			fprintf(stderr, MSG_FILE_ERROR, "read", strerror(errno), errno);
			main_rc = -1;
			goto end;
		}
		if(read_size == 0) {  /* 読み込むデータがなくなった */
			break;
		}

		buff[read_size] = '\0';
		/* sendを最大MAX_SEND_RETRY回リトライする */
		for(retry_count = 0;retry_count < MAX_SEND_RETRY; retry_count++) {
			send_size = send(sock_fd, buff, read_size, 0);
			if(send_size > 0) {
				break;
			}
			fprintf(stderr, MSG_SEND_RETRY);
		}
		if(send_size == -1) {
			fprintf(stderr, MSG_SEND_ERROR, strerror(errno), errno);
			main_rc = -1;
			goto end;
		}
	}

end:
	if(sock_fd > 0) {
		rc = close(sock_fd);
		if(rc == -1) {
			fprintf(stderr, MSG_CLOSE_ERROR, "socket", strerror(errno), errno);
			main_rc = -1;
		}
	}
	if(infile_fd > 0) {
		rc= close(infile_fd);
		if(rc == -1) {
			fprintf(stderr, MSG_FILE_ERROR, "close", strerror(errno), errno);
			main_rc = -1;
		}
	}
	return main_rc;
}

/**
 * @brief 配信のソケットを準備し、ファイルディスクリプタを返す
 * 
 * @param[in] hostname  接続先ホスト名
 * @param[in] portno    接続先ポート番号
 *
 * @retval > 0 作成したソケット
 * @retval -1  ソケット作成失敗
 */
static int send_connect(char *hostname, unsigned short portno)
{
	int is_fail = 0;
	int rc;
	int sock_fd;
	struct sockaddr_in server_addr;
	struct hostent *server;

	/* ソケット作成(IP V4, TCP, プロトコルは自動選択) */
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd == -1) {
		fprintf(stderr, MSG_SOCKET_ERROR, strerror(errno), errno);
		return -1;
	}
	
	/* ホスト名の解決 */
	server = gethostbyname(hostname);
	if(server == NULL) {
		fprintf(stderr, MSG_NAME_RESOL_ERROR, strerror(h_errno), h_errno);
		is_fail = 1;
		goto end;
	}

	/* アドレスの設定 */
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portno);
	server_addr.sin_addr = *((struct in_addr*)server->h_addr);

	/* 接続 */
	rc = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	if(rc == -1) {
		fprintf(stderr, MSG_CONNECT_ERROR, strerror(errno), errno);
		is_fail = 1;
		goto end;
	}
end:
	/* 失敗時の後始末 */
	if(is_fail) {
		if(sock_fd > 0) {
			rc = close(sock_fd);
			if(rc == -1) {
				fprintf(stderr, MSG_CLOSE_ERROR, "socket", strerror(errno), errno);
			}
		}
		return -1;
	}
	return sock_fd;
}

