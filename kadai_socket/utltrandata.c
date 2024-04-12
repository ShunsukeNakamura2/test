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

#define MSG_ERR_USAGE \
	"usage:\n"\
	"utltrandata -s portno -h hostname -f filename\n"\
	"	-h hostname	指定されたホストに対して接続します。\n"\
	"	-s portno	指定されたポート番号に対して接続します。\n"\
	"	-f filename	指定されたファイルからデータを読み込みます。\n\n"\
	"utltrandata -r portno -f filename\n"\
	"	-r portno	指定されたポート番号で接続を待ちます。\n"\
	"	-f filename	指定されたファイルにデータを追記します。\n"
#define MSG_ERR_COMMON "error:%s (code:%d)\n"
#define MSG_ERR_FILE "file %s error\n"MSG_ERR_COMMON
#define MSG_ERR_FORK "fork error\n"MSG_ERR_COMMON
#define MSG_ERR_GET_OPEN_MAX "failed to get file descriptor openmax\n"MSG_ERR_COMMON
#define MSG_ERR_SOCKET "failed to socket\n"MSG_ERR_COMMON
#define MSG_ERR_BIND "failed to bind\n"MSG_ERR_COMMON
#define MSG_ERR_LISTEN "failed to listen\n"MSG_ERR_COMMON
#define MSG_ERR_ACCEPT "failed to accept\n"MSG_ERR_COMMON
#define MSG_ERR_SEND "failed to send\n"MSG_ERR_COMMON
#define MSG_ERR_RECV "failed to recv\n"MSG_ERR_COMMON
#define MSG_ERR_NAME_RESOL "failed to host name resolution\n"MSG_ERR_COMMON
#define MSG_ERR_CONNECT "failed to connect\n"MSG_ERR_COMMON
#define MSG_ERR_SELECT "failed to select\n"MSG_ERR_COMMON
#define MSG_ERR_DAEMON_INIT "failed to daemonize process\n"
#define MSG_ERR_SEND_PROCESS "failed to send process\n"
#define MSG_ERR_RECV_PROCESS "failed to receive process\n"
#define MSG_ERR_SEND_RETRY "failed to send > retry...\n"
#define MSG_ERR_RECV_RETRY "failed to receive > retry...\n"
#define MSG_ERR_SEND_TIMEOUT "send:socket timeout\n"
#define MSG_ERR_RECV_TIMEOUT "recv:socket timeout\n"
#define MSG_ERR_USE_WELL_KNOUN_PORT "cannot use well known port\n"
#define RETURN_NORMAL_END 0
#define RETURN_ERR_USAGE 1
#define RETURN_ERR_DAEMONIZE 2
#define RETURN_ERR_SEND 3
#define RETURN_ERR_RECV 4
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
static int init_daemon();
static int run_sending_proc(st_args params);
static int run_receiving_proc(st_args params);
static int prepare_recv_socket(unsigned short portno);
static int recv_data(int client_fd, char *filename);
static int send_connect(char *hostname, unsigned short portno);

int main(int argc, char *argv[]) {
	int rc;
	st_args params = {0};

	rc = analyze_args(argc, argv, &params);
	if(rc == -1) {
		fprintf(stderr, MSG_ERR_USAGE);
		return RETURN_ERR_USAGE;
	}

	switch(params.mode) {
	case MODE_SEND:
		rc = run_sending_proc(params);
		if(rc == -1) {
			fprintf(stderr, MSG_ERR_SEND_PROCESS);
			return RETURN_ERR_SEND;
		}
		break;
	case MODE_RECV:
		rc = init_daemon();
		if(rc == -1) {
			fprintf(stderr, MSG_ERR_DAEMON_INIT);
			return RETURN_ERR_DAEMONIZE;
		}
		rc = run_receiving_proc(params);
		if(rc == -1) {
			fprintf(stderr, MSG_ERR_RECV_PROCESS);
			return RETURN_ERR_RECV;
		}
		break;
	default:
		break;
	}
	return RETURN_NORMAL_END;
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
	int tmp_portno;

	if(argc < ARGC_MIN ) {
		return -1;
	}

	/* オプションとパラメータを取得 */
	for(i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-h") == 0) {
			/* ホスト名 */
			if(params->hostname != NULL) {
				return -1;
			}
			if(++i >= argc) {
				return -1;
			}
			params->hostname = argv[i];
		} else if(strcmp(argv[i], "-f") == 0) {
			/* ファイル名 */
			if(params->filename != NULL) {
				return -1;
			}
			if(++i >= argc) {
				return -1;
			}
			params->filename = argv[i];
		} else if(strcmp(argv[i], "-s") == 0) {
			/* 送信モード＆ポート番号 */
			if(params->mode != 0) {
				return -1;
			}
			if(++i >= argc) {
				return -1;
			}
			params->mode = MODE_SEND;
			portno_str = argv[i];
		} else if(strcmp(argv[i], "-r") == 0) {
			/* 受信モード＆ポート番号 */
			if(params->mode != 0) {
				return -1;
			}
			if(++i >= argc) {
				return -1;
			}
			params->mode = MODE_RECV;
			portno_str = argv[i];
		}
	}

	/* パラメータに過不足がある場合引数不正 */
	switch(params->mode) {
	case MODE_SEND:
		if(params->hostname == NULL || params->filename == NULL || portno_str == NULL) {
			return -1;
		}
		break;
	case MODE_RECV:
		/* 受信モードではhostnameを指定してはいけない */
		if(params->hostname != NULL || params->filename == NULL || portno_str == NULL) {
			return -1;
		}
		break;
	default:
		return -1;
	}

	tmp_portno = (int)strtol(portno_str, &endptr, 10);
	/* 文字列 -> 数値 変換失敗 */
	if(*endptr != '\0') {
		return -1;
	}
	/* 範囲外のport番号 */
	if(tmp_portno < 0 || tmp_portno > 65535) {
		return -1;
	}
	/* Well Known Ports が指定されていないかチェック */
	if(0 <= tmp_portno && tmp_portno <= 1023) {
		fprintf(stderr, MSG_ERR_USE_WELL_KNOUN_PORT);
		return -1;
	}
	params->portno = (unsigned short)tmp_portno;

	return 0;
}

/**
 * @brief プロセスのデーモン化を行う
 *
 * @retval 0  デーモン化成功
 * @retval -1 デーモン化失敗
 */
static int init_daemon()
{
	pid_t pid;
	int rep_num;
	int i;

	pid = fork();
	if(pid < 0) {
		fprintf(stderr, MSG_ERR_FORK, strerror(errno), errno);
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
		fprintf(stderr, MSG_ERR_GET_OPEN_MAX, strerror(errno), errno);
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
 * @retval -1 異常終了
 */
static int run_receiving_proc(st_args params)
{
	int sock_fd = -1;
	int client_fd = -1;
	int rc;
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	/* 受信用のソケットを準備 */
	sock_fd = prepare_recv_socket(params.portno);
	if(sock_fd == -1) {
		return -1;
	}

	while(1) {
		/* 接続を待つ */
		client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if(client_fd == -1) {
			fprintf(stderr, MSG_ERR_ACCEPT, strerror(errno), errno);
			goto end;
		}

		/* データを受信してファイルに書き込む */
		rc = recv_data(client_fd, params.filename);
		if(rc == -1) {
			goto end;
		}

		/* ソケットを閉じて次の接続へ */
		close(client_fd);
	}

end:
	/* fdを開いていたら閉じる */
	if(client_fd >= 0) {
		close(client_fd);
	}
	close(sock_fd);

	return -1;
}

/**
 * @brief 受信側のソケットを準備し、ファイルディスクリプタを返す
 *
 * @param[in] portno    使用するポート番号
 * 
 * @retval > 0 作成したソケット
 * @retval -1  ソケット作成失敗
 */
static int prepare_recv_socket(unsigned short portno)
{
	int sock_fd;
	int rc;
	struct sockaddr_in recv_addr;

	/* ソケット作成(IP V4, TCP, プロトコルは自動選択) */
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd == -1) {
		fprintf(stderr, MSG_ERR_SOCKET, strerror(errno), errno);
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
		fprintf(stderr, MSG_ERR_BIND, strerror(errno), errno);
		goto end;
	}

	/* ソケットを接続待ちに設定 接続要求を最大5件まで待たせる */
	rc = listen(sock_fd, 5);
	if(rc == -1) {
		fprintf(stderr, MSG_ERR_LISTEN, strerror(errno), errno);
		goto end;
	}
end:
	if(rc == -1) {
		close(sock_fd);
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
	int outfile_fd;
	char buff[BUFF_SIZE];
	int rc;
	int retry_count;
	ssize_t size = 0;
	fd_set rfds; /* 読み込み可能か監視するfdの集合 */
	struct timeval timeout;

	outfile_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(outfile_fd == -1) {
		fprintf(stderr, MSG_ERR_FILE, "open", strerror(errno), errno);
		return -1;
	}

	while(1) {
		memset(buff, 0, BUFF_SIZE);
		/* recvを最大MAX_RECV_RETRY回リトライする */
		for(retry_count = 0; retry_count < MAX_RECV_RETRY; retry_count++) {
			/* ソケットが読み込み可能か確認 */
			FD_ZERO(&rfds);
			FD_SET(client_fd, &rfds);
			timeout.tv_sec = TIMEOUT_SEC;
			timeout.tv_usec = 0;
			rc = select(client_fd + 1, &rfds, NULL, NULL, &timeout);
			if(rc == -1) {
				fprintf(stderr, MSG_ERR_SELECT, strerror(errno), errno);
				goto end;
			} else if(rc == 0) {
				fprintf(stderr, MSG_ERR_RECV_TIMEOUT);
				rc = -1;
				continue;
			}

			size = recv(client_fd, buff, BUFF_SIZE, 0);
			if(size >= 0) {
				break;
			}
			rc = -1;
			if(errno == EINTR || errno == EAGAIN) {  /* retryしてもいいエラー */
				fprintf(stderr, MSG_ERR_RECV_RETRY);
				continue;
			}
			/* retryしても復旧できないエラー */
			fprintf(stderr, MSG_ERR_RECV, strerror(errno), errno);
			goto end;
		}

		if(size == 0) { /* 相手が接続を終わったと判断 */
			break;
		}

		size = write(outfile_fd, buff, size);
		if(size == -1) {
			fprintf(stderr, MSG_ERR_FILE, "write", strerror(errno), errno);
			rc = -1;
			goto end;
		}
	}

end:
	close(outfile_fd);
	return rc == -1 ? -1 : 0;
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
static int run_sending_proc(st_args params)
{
	int sock_fd = -1;
	int infile_fd = -1;
	int rc = 0;
	int retry_count;
	ssize_t read_size;
	ssize_t send_size;
	char buff[BUFF_SIZE];
	fd_set wfds; /* 書き込み可能か監視するfdの集合 */
	struct timeval timeout;

	/* 送信用のソケットを準備 */
	sock_fd = send_connect(params.hostname, params.portno);
	if(sock_fd == -1) {
		return -1;
	}
	
	infile_fd = open(params.filename, O_RDONLY);
	if(infile_fd == -1) {
		fprintf(stderr, MSG_ERR_FILE, "open", strerror(errno), errno);
		rc = -1;
		goto end;
	}

	while(1) {
		read_size = read(infile_fd, buff, BUFF_SIZE);
		if(read_size == -1) {
			fprintf(stderr, MSG_ERR_FILE, "read", strerror(errno), errno);
			rc = -1;
			goto end;
		} else if(read_size == 0) {  /* 読み込むデータがなくなった */
			break;
		}

		/* sendを最大MAX_SEND_RETRY回リトライする */
		for(retry_count = 0;retry_count < MAX_SEND_RETRY; retry_count++) {
			/* ソケットが書き込み可能か確認 */
			FD_ZERO(&wfds);
			FD_SET(sock_fd, &wfds);
			timeout.tv_sec = TIMEOUT_SEC;
			timeout.tv_usec = 0;
			rc = select(sock_fd + 1, NULL, &wfds, NULL, &timeout);
			if(rc == -1) {
				fprintf(stderr, MSG_ERR_SELECT, strerror(errno), errno);
				goto end;
			} else if(rc == 0) {
				fprintf(stderr, MSG_ERR_SEND_TIMEOUT);
				rc = -1;
				continue;
			}

			send_size = send(sock_fd, buff, read_size, 0);
			if(send_size > 0) {
				break;
			}
			rc = -1;
			if(errno == EINTR || errno == EAGAIN) { /* retryしてもいいエラー */
				fprintf(stderr, MSG_ERR_SEND_RETRY);
				continue;
			}
			/* retryしても復旧しないエラー */
			fprintf(stderr, MSG_ERR_SEND, strerror(errno), errno);
			goto end;
		}
		
	}

end:

	if(infile_fd >= 0) {
		close(infile_fd);
	}
	close(sock_fd);
	return rc == -1 ? -1 : 0;
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
	int rc;
	int sock_fd;
	struct sockaddr_in server_addr;
	struct hostent *server;

	/* ソケット作成(IP V4, TCP, プロトコルは自動選択) */
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd == -1) {
		fprintf(stderr, MSG_ERR_SOCKET, strerror(errno), errno);
		return -1;
	}
	
	/* ホスト名の解決 */
	server = gethostbyname(hostname);
	if(server == NULL) {
		fprintf(stderr, MSG_ERR_NAME_RESOL, strerror(h_errno), h_errno);
		rc = -1;
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
		fprintf(stderr, MSG_ERR_CONNECT, strerror(errno), errno);
		goto end;
	}

end:	
	if(rc == -1) {
		close(sock_fd);
		return -1;
	}
	return sock_fd;
}

