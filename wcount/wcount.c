#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

/*
 * 〇最終課題
 * ファイルに含まれる単語と出現回数を表示
 */

#define MSG_ERR_USAGE \
	"usage : wcount [-o database] –i infile\n"\
	"        wcount –r database\n"
#define MSG_ERR_MEM_ALLOCATE "memory allocation error.\n"
#define MSG_ERR_FILE_OPEN "file open error. [%s][%s]\n"
#define MSG_ERR_INFILE_FORMAT "invalid file format. [%s]\n"
#define RC_NORMAL_END		0
#define RC_ERR_ARG 		1
#define RC_ERR_FILE_OPEN 	2
#define RC_ERR_INFILE_FORMAT 	3
#define RC_ERR_DB_FORMAT	4
#define RC_ERR_MEM_ALLOCATE	5
#define RC_ERR_SYSTEM		6
#define MODE_INPUT 1
#define MODE_DISP  2
#define INFILE_LINE_MAX 1024

/* 引数解析後のパラメータを入れる構造体 */
typedef struct CmdParams_t {
	int mode;	/* 動作モード */
	int db_flag;	/* 入力モード時 -o database オプションが指定されているか */
	char *infile;	/* -i infile で渡される入力ファイルへのパス */
	char *database;	/* -o(-r) database で渡されるDBファイルへのパス */
} CmdParams;

typedef struct WordData_t {
	char *word;		/* 単語が格納されたメモリへのポインタ */
	int count;		/* 単語の出現回数 */
	struct WordData_t *next;/* 次の単語データ(WordData)へのポインタ */
} WordData;

/* proc_input_request()で使用する(初期化・終了処理が必要な)データをまとめた構造体 */
typedef struct InputModeData_t {
	WordData *root;
	int fd_database;
	int fd_infile;
} InputModeData;

/* proc_disp_request()で使用する(初期化が必要な)データをまとめた構造体 */
typedef struct DispModeData_t {
	WordData *root;
	int fd_database;
} DispModeData;

/* 一単語を表す構造体 */
typedef struct Token_t {
	char *ptr;	/* 「単語」へのポインタ */
	int length;	/* 単語の長さ(\0を含む) */
} Token;

static int analyze_args(int argc, char *argv[], CmdParams *params);
/* 入力モード時処理 */
static int proc_input_request(CmdParams params);
static int init_proc_input_request(InputModeData *init_data, CmdParams params);
static void finalize_proc_input_request(InputModeData *finalize_data);
/* DB表示モード時処理 */
static int proc_disp_request(CmdParams params);
static int init_proc_disp_request(DispModeData *init_data, CmdParams params);
static void finalize_proc_disp_request(DispModeData *finalize_data);
/* WordDataリスト操作関連 */
static int load_WDList_from_database(int fd_database, WordData *root);
static int save_WDList_to_database(int fd_database, WordData *root);
static int make_WDList_from_infile(int fd_infile, char *infilename, WordData *root);
static WordData *create_WD(Token *token, int count);
static WordData *init_WDList();
static void disp_WDList(WordData *root);
static void free_WDList(WordData *root);
static void add_to_WDList(WordData *new_word, WordData *root);
/* utility */
static int search_last_newline(char *str, int length);
static void get_token(char *str, Token *token);

int main(int argc, char *argv[])
{
	int rc;
	CmdParams params = {0};

	/* 引数解析 */
	rc = analyze_args(argc, argv, &params);
	if(rc == -1) {
		fprintf(stderr, MSG_ERR_USAGE);
		return RC_ERR_ARG;
	}

	switch(params.mode) {
	case MODE_INPUT:
		rc = proc_input_request(params);
		break;
	case MODE_DISP:
		rc = proc_disp_request(params);
		break;
	default:
		break;
	}

	return rc;
}

/**
 * @bfief コマンドライン引数をチェックしパラメータをparamsに入れる
 *
 * @param[in] argc コマンドライン引数の個数
 * @param[in] argv コマンドライン引数
 * @param[in,out] params パラメータ格納用構造体
 *
 * @retval 0   正常終了
 * @retval -1  コマンドライン引数不正
 */
static int analyze_args(int argc, char *argv[], CmdParams *params)
{
	int i;
	
	for(i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-i") == 0){
			/* 入力モード */
			if(params->mode != 0) {
				return -1;
			}
			if(++i >= argc) {
				return -1;
			}
			params->mode = MODE_INPUT;
			params->infile = argv[i];
		}
		if(strcmp(argv[i], "-r") == 0){
			/* DB表示モード */
			if(params->mode != 0) {
				return -1;
			}
			if(++i >= argc) {
				return -1;
			}
			params->mode = MODE_DISP;
			params->database = argv[i];
		}
		if(strcmp(argv[i], "-o") == 0){
			/* 入力モード時のdatabase */
			if(params->db_flag != 0) {
				return -1;
			}
			if(++i >= argc) {
				return -1;
			}
			params->db_flag = 1;
			params->database = argv[i];
		}
	}
	
	switch(params->mode) {
	case MODE_INPUT:
		break;
	case MODE_DISP:
		/* –rと–oは同時に指定してはいけない */
		if(params->db_flag) {
			return -1;
		}
		break;
	default:
		/* modeが設定されていない */
		return -1;
	}

	return 0;
}

/**
 * @brief 入力モード時の処理
 *
 * @param[in] params コマンドライン引数の解析済パラメータ
 *
 * @retval RC_NORMAL_END        正常終了
 * @retval RC_ERR_FILE_OPEN	ファイルオープンエラー
 * @retval RC_ERR_INFILE_FORMAT 入力ファイルフォーマットエラー
 * @retval RC_ERR_DB_FORMAT     databaseファイルフォーマットエラー
 * @retval RC_ERR_MEM_ALLOCATE  メモリ確保エラー
 * @retval RC_ERR_SYSTEM        その他、致命的なエラー
 */
static int proc_input_request(CmdParams params)
{
	int rc;
	InputModeData initialized_vars;

	rc = init_proc_input_request(&initialized_vars, params);
	if(rc != RC_NORMAL_END) {
		return rc;
	}

	/* database読み込み */
	/* 未 */
	if(params.db_flag) {
		rc = load_WDList_from_database(initialized_vars.fd_database, initialized_vars.root);
	}
	if(rc != RC_NORMAL_END) {}

	/* infile読み込み */
	rc = make_WDList_from_infile(initialized_vars.fd_infile, params.infile, initialized_vars.root);
	if(rc != RC_NORMAL_END) {
		goto end;
	}

	if(params.db_flag) {
		/* databaseへ出力 */
		/* 未 */
		rc = save_WDList_to_database(initialized_vars.fd_database, initialized_vars.root);
		if(rc != RC_NORMAL_END) {}
	} else {
		/* 標準出力 */
		disp_WDList(initialized_vars.root);
	}
end:
	finalize_proc_input_request(&initialized_vars);

	return rc;
}

/**
 * @brief proc_input_request()で使用するfdのopne、構造体リストの作成を行う
 *
 * @param[in,out] init_data  関数内で使用する初期化が必要な変数をまとめた構造体
 * @param[in]     params     コマンドライン引数の解析済みパラメータ
 *
 * @retval RC_NORMAL_END        正常終了
 * @retval RC_ERR_FILE_OPEN     ファイルオープンエラー
 * @retval RC_ERR_MEM_ALLOCATE  メモリ確保エラー
 */
static int init_proc_input_request(InputModeData *init_data, CmdParams params)
{
	init_data->root = init_WDList();
	if(init_data->root == NULL) {
		fprintf(stderr, MSG_ERR_MEM_ALLOCATE);
		return RC_ERR_MEM_ALLOCATE;
	}

	init_data->fd_infile = open(params.infile, O_RDONLY);
	if(init_data->fd_infile == -1) {
		fprintf(stderr, MSG_ERR_FILE_OPEN, strerror(errno), params.infile);
		finalize_proc_input_request(init_data);
		return RC_ERR_FILE_OPEN;
	}

	if(!params.db_flag) {
		return RC_NORMAL_END;
	}

	init_data->fd_database = open(params.database, O_RDWR | O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(init_data->fd_database == -1) {
		fprintf(stderr, MSG_ERR_FILE_OPEN, strerror(errno), params.database);
		finalize_proc_input_request(init_data);
		return RC_ERR_FILE_OPEN;
	}

	return RC_NORMAL_END;
}

/**
 * @brief proc_input_request()で使用したfdのclose、構造体リストの解放を行う
 *
 * @param[in] finalize_data  関数内で使用した、終了処理が必要な変数をまとめた構造体
 */
static void finalize_proc_input_request(InputModeData *finalize_data)
{
	free_WDList(finalize_data->root);
	if(finalize_data->fd_infile > 0) {
		close(finalize_data->fd_infile);
	}
	if(finalize_data->fd_database > 0) {
		close(finalize_data->fd_database);
	}
	return;
}

/**
 * @brief DB表示モード時の処理
 *
 * @param[in] params コマンドライン引数の解析済パラメータ
 *
 * @retval RC_NORMAL_END	正常終了
 * @retval RC_ERR_FILE_OPEN     ファイルオープンエラー
 * @retval RC_ERR_DB_FORMAT	databaseファイルフォーマットエラー
 * @retval RC_ERR_MEM_ALLOCATE  メモリ確保エラー
 * @retval RC_ERR_SYSTEM	その他、致命的なエラー
 */
static int proc_disp_request(CmdParams params)
{
	int rc;
	DispModeData initialized_vars;
	
	rc = init_proc_disp_request(&initialized_vars, params);
	if(initialized_vars.root == NULL) {}
	
	/* read database */
	rc = load_WDList_from_database(initialized_vars.fd_database, initialized_vars.root);
	if(rc != RC_NORMAL_END) {}

	/* disp all words(stdout) */
	disp_WDList(initialized_vars.root);
	
	finalize_proc_disp_request(&initialized_vars);
	
	return 0;
}

/**
 * @brief proc_disp_request()で使用するfdのopne、構造体リストの作成を行う
 *
 * @param[in,out] init_data  関数内で使用する初期化が必要な変数をまとめた構造体
 * @param[in]     params     コマンドライン引数の解析済みパラメータ
 *
 * @retval RC_NORMAL_END        正常終了
 * @retval RC_ERR_FILE_OPEN     ファイルオープンエラー
 * @retval RC_ERR_MEM_ALLOCATE  メモリ確保エラー
 */
static int init_proc_disp_request(DispModeData *init_data, CmdParams params)
{
	return 0;
}

/**
 * @brief proc_input_request()で使用したfdのclose、構造体リストの解放を行う
 *
 * @param[in] finalize_data  関数内で使用した、終了処理が必要な変数をまとめた構造体
 */
static void finalize_proc_disp_request(DispModeData *finalize_data)
{
	return;
}

/**
 * @brief databaseから単語データを読み込んでWordDataのリストに格納する
 * 
 * @param[in]	  fd_database  読み込み対象のdatabaseのファイルディスクリプタ
 * @param[in,out] root         WordDataリストの先頭要素のアドレス
 *
 * @retval RC_NORMAL_END        正常終了
 * @retval RC_ERR_FILE_OPEN     ファイルオープンエラー
 * @retval RC_ERR_DB_FORMAT     databaseファイルフォーマットエラー
 * @retval RC_ERR_MEM_ALLOCATE  メモリ確保エラー
 * @retval RC_ERR_SYSTEM        その他、致命的なエラー
 */
static int load_WDList_from_database(int fd_database, WordData *root)
{	
	WordData *new_word;

	
	/* databaseを全て読み込み */
	while(0) {
		/* 単語を一つ取得 */
		new_word = create_WD(NULL, 0);
		if(new_word == NULL) {}

		/* 取得した単語をリストに追加 */
		add_to_WDList(root, new_word);
	}

	return 0;
}

/**
 * @brief WordDataリストの単語を、フォーマットに従いdatabaseに書き込む
 *
 * @param[in] fd_database  書き込み対象databaseのファイルディスクリプタ
 * @param[in] WordData     WordDataリストの先頭要素のアドレス
 *
 * @retval RC_NORMAL_END     正常終了
 * @retval RC_ERR_FILE_OPEN  ファイルオープンエラー
 * @retval RC_ERR_SYSTEM     その他、致命的なエラー
 */
static int save_WDList_to_database(int fd_database, WordData *root)
{
	return 0;
}

/**
 * @brief 入力ファイルの単語をカウントし、WordDataのリストに格納する
 *
 * @param[in]	  fd_infile   読み込み対象ファイルのファイルディスクリプタ
 * @para,[in]	  infilename  読み込み対象のファイル名
 * @param[in,out] root	      WordDataリストの先頭要素のアドレス
 *
 * @retval RC_NORMAL_END        正常終了
 * @retval RC_ERR_FILE_OPEN     ファイルオープンエラー
 * @retval RC_ERR_INFILE_FORMAT 入力ファイルフォーマットエラー
 * @retval RC_ERR_MEM_ALLOCATE  メモリ確保エラー
 * @retval RC_ERR_SYSTEM        その他、致命的なエラー
 */
static int make_WDList_from_infile(int fd_infile,char *infilename , WordData *root)
{
	WordData *new_word;
	Token token;
	char read_str[INFILE_LINE_MAX] = {0};
	char *word;
	int i;
	int skipped_str_len = 0;
	int last_newline_pos = 0;
	ssize_t size;
	
	while(1) {
		/* スキップした文字列を、先頭に持ってきて、空いた分だけread */
		memmove(read_str, read_str + last_newline_pos + 1, skipped_str_len);
		memset(read_str + skipped_str_len, 0, INFILE_LINE_MAX - skipped_str_len);
		size = read(fd_infile, read_str + skipped_str_len, INFILE_LINE_MAX - skipped_str_len);
		if(size == 0) {
			break;
		}
		/* read_str中の最後の改行以降の文字列は、今回は処理をスキップ */
		last_newline_pos = search_last_newline(read_str, (int)size + skipped_str_len);
		/* 1行の制限越え */
		if(last_newline_pos == -1) {
			fprintf(stderr, MSG_ERR_INFILE_FORMAT, infilename);
			return RC_ERR_INFILE_FORMAT;
		}
		skipped_str_len = INFILE_LINE_MAX - last_newline_pos - 1;
		read_str[last_newline_pos] = '\0';

		/* 単語に区切ってリストに格納 */
		get_token(read_str, &token);
		while(1) {
			if(token.ptr == NULL) {
				break;
			}
			new_word = create_WD(&token, 1);
			if(new_word == NULL) {
				fprintf(stderr, MSG_ERR_MEM_ALLOCATE);
				return RC_ERR_MEM_ALLOCATE;
			}
			add_to_WDList(new_word, root);
			get_token(NULL, &token);
		}
	}
	return 0;
}

/**
 * @brief 新しいWordDataを作り、そのポインタを返す
 *
 * @param[in] token  WordDataのメンバwordに格納する単語
 * @param[in] count  WordDataのメンバcountに格納する数値
 *
 * @return 成功時：作成したWordData構造体のアドレス
 *         失敗時：NULL
 */
static WordData *create_WD(Token *token, int count)
{
	WordData *new_WD = (WordData*)calloc(sizeof(WordData), 1);
	if(new_WD == NULL) {
		return NULL;
	}

	/* rootの場合はwordへのメモリ割り当てを行わない */
	if(token == NULL) {
		return new_WD;
	}

	new_WD->word = (char*)malloc(token->length);
	if(new_WD->word == NULL) {
		return NULL;
	}

	memcpy(new_WD->word, token->ptr, token->length);
	new_WD->count = count;
	return new_WD;
}

/**
 * @brief 新しいWordDataのリストを作成し、先頭要素のアドレスを返す
 *
 * @return 成功時：作成したWordData構造体リストの先頭要素のアドレス
 * 	   失敗時：NULL
 */
static WordData *init_WDList()
{
	return create_WD(NULL, 0);
}

/**
 * @brief WordDataリストの中身をフォーマットに従い標準出力で表示する
 *
 * @param[in] root   WordDataリストの先頭要素のアドレス
 */
static void disp_WDList(WordData *root)
{
	WordData *current = root->next;
	while(1) {
		if(current == NULL) {
			break;
		}
		printf("%-20s%d\n", current->word, current->count);
		current = current->next;
	}
	return;
}

/**
 * @brief WordDataリスト用に確保したメモリを解放
 *
 * @param[in]   対象のリストの先頭要素のアドレス
 */
static void free_WDList(WordData *root)
{
	WordData *current = root;
	WordData *next;

	while(1) {
		if(current == NULL) {
			break;
		}

		next = current->next;
		free(current->word);
		free(current);
		current = next;
	}
	return;
}

/**
 * @brief WordDataリストに新しい単語を追加
 * 
 * @param[in]    new_word  追加対象のWordDataのアドレス
 * @param[in,out] root      new_word追加先のWordDataリストの先頭要素アドレス
 */
static void add_to_WDList(WordData *new_word, WordData *root)
{
	WordData *prev = root;
	WordData *current = root->next;
	int cmp_rslt;
	while(1) {
		if(current == NULL) {/* 末尾に追加 */
			prev->next = new_word;
			break;
		}
		cmp_rslt = strcmp(current->word, new_word->word);

		if(cmp_rslt == 0) {/* 同じ単語なので合算 */
			current->count += new_word->count;
			free(new_word->word);
			free(new_word);
			break;
		} else if(cmp_rslt > 0) {/* prevとcurrentの間に挿入 */
			prev->next = new_word;
			new_word->next = current;
			break;
		}
		
		prev = current;
		current = current->next;
	}
	return;
}

/**
 * @brief 文字列の中で最後に出てくる改行を探して、先頭からの距離を返す
 *
 * @param[in] str     検索対象の文字列
 * @param[in] length  strの長さ
 *
 * @retval >=0 改行コードの、先頭からの距離
 * @retval -1  改行コードがない 
 */
static int search_last_newline(char *str, int length)
{
	int i;
	for(i = length - 1; i >= 0; i--) {
		if(str[i] == '\n') {
			return i;
		}
	}
	return -1;
}

/**
 * @brief 文字列から単語を取り出しtokenに詰める
 *
 * @param[in,out] str    解析対象の文字列
 * 		         NULLを渡すと前回呼び出し時の文字列の続きから解析を行う
 * @param[in,out] token  解析結果を詰める構造体
 *                       トークンがない場合,ptrにNULLを入れて返す
 *                         
 * @note 1.引数strはこの関数内で変更される
 *       2.単語の定義：英数字および「'」が連続している文字列
 */
static void get_token(char *str, Token *token)
{
	static char *head_ptr;
	int length = 0;
	
	/* 新規文字列 */
	if(str != NULL) {
		head_ptr = str;
	}

	if(*head_ptr == '\0') {
		token->ptr = NULL;
		token->length = 0;
		return;
	}

	token->ptr = head_ptr;
	while(1) {
		if(!isalnum((int)*head_ptr) && *head_ptr != '\'') {
			break;
		}
		head_ptr++;
		length++;
	}

	/* \0も含めるため1文字足す */
	token->length = length + 1;
	if(*head_ptr == '\0') {
		return;
	}
	*head_ptr = '\0';
	/* 次に備えて次の単語の頭までhead_ptrを進める */
	while(1) {
		head_ptr++;
		if(*head_ptr == '\0') {
			return;
		} else if(isalnum((int)*head_ptr) || *head_ptr == '\'') {
			return;
		}
		*head_ptr = '\0';
	}
	return;
}

