#include <stdio.h>

/*
 * 〇最終課題
 * ファイルに含まれる単語と出現回数を表示
 */

#define MODE_INPUT 1
#define MODE_DISP  2
#define RC_NORMAL_END		0
#define RC_ARG_ERROR 		1
#define RC_FILE_OPEN_ERROR 	2
#define RC_INFILE_FORMAT_ERROR 	3
#define RC_DB_FORMAT_ERROR	4
#define RC_MEM_ALLOCATE_ERROR	5
#define RC_SYSTEM_ERROR		6

typedef struct st_args {
	int mode;
	int db_flag;
	char *infile;
	char *database;
} st_args;

typedef struct WordData_t {
	char *word;		/* 単語が格納されたメモリへのポインタ */
	int count;		/* 単語の出現回数 */
	struct WordData_t *next;/* 次の単語データ(WordData)へのポインタ */
} WordData;


static int analyze_args(int argc, char *argv[], st_args *params);
static int input_mode_process(st_args params);
static int disp_mode_process(st_args params);

/* リスト操作関連 */
static int read_database(char *database, WordData *root);
static int write_database(char *database, WordData *root);
static int read_input_file(char *filename, WordData *root);
static WordData* make_new_word(char *word, int count);
static void disp_all_words(WordData *root);
static void free_list(WordData *root);
static void add_word(WordData *new_word, WordData *root);

int main(int argc, char *argv[])
{
	int rc;
	st_args params;

	/* 引数解析 */
	rc = analyze_args(argc, argv, &params);
	if(rc == -1){}

	switch(params.mode) {
	case MODE_INPUT:
		rc = input_mode_process(params);
		break;
	case MODE_DISP:
		rc = disp_mode_process(params);
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
static int analyze_args(int argc, char *argv[], st_args *params)
{
	return 0;
}

/**
 * @brief 入力モード時の処理
 *
 * @param[in] params コマンドライン引数の解析済データ
 *
 * @retval RC_NORMAL_END          正常終了
 * @retval RC_FILE_OPEN_ERROR	  ファイルオープンエラー
 * @retval RC_INFILE_FORMAT_ERROR 入力ファイルフォーマットエラー
 * @retval RC_DB_FORMAT_ERROR     databaseファイルフォーマットエラー
 * @retval RC_MEM_ALLOCATE_ERROR  メモリ確保エラー
 * @retval RC_SYSTEM_ERROR        その他、致命的なエラー
 */
static int input_mode_process(st_args params)
{
	int rc = RC_NORMAL_END;
	WordData *root;
	root = make_new_word(NULL, 0);
	if(root == NULL) {}

	/* read database */
	if(params.db_flag) {
		rc = read_database(params.database, root);
	}
	if(rc != RC_NORMAL_END) {}

	/* read infile */
	rc = read_input_file(params.infile, root);
	if(rc != RC_NORMAL_END) {}

	/* output data */
	if(params.db_flag) {
		/* write database */
		rc = write_database(params.database, root);
		if(rc != RC_NORMAL_END) {}
	} else {
		/* disp all words(stdout) */
		disp_all_words(root);
	}

	/* clear list */
	free_list(root);

	return 0;
}

/**
 * @brief DB表示モード時の処理
 *
 * @param[in] params コマンドライン引数の解析済データ
 *
 * @retval RC_NORMAL_END	  正常終了
 * @retval RC_FILE_OPEN_ERROR     ファイルオープンエラー
 * @retval RC_DB_FORMAT_ERROR	  databaseファイルフォーマットエラー
 * @retval RC_MEM_ALLOCATE_ERROR  メモリ確保エラー
 * @retval RC_SYSTEM_ERROR	  その他、致命的なエラー
 */
static int disp_mode_process(st_args params)
{
	int rc = RC_NORMAL_END;
	WordData *root;
	root = make_new_word(NULL, 0);
	if(root == NULL) {}
	
	/* read database */
	rc = read_database(params.database, root);
	if(rc != RC_NORMAL_END) {}

	/* disp all words(stdout) */
	disp_all_words(root);
	
	/* リストのメモリを解放 */
	free_list(root);

	return 0;
}

/**
 * @brief databaseから単語データを読み込んでWordDataのリストに格納する
 * 
 * @param[in]	  database  読み込み対象のdatabase
 * @param[in,out] root      WordDataリストの先頭要素のアドレス
 *
 * @retval RC_NORMAL_END          正常終了
 * @retval RC_FILE_OPEN_ERROR     ファイルオープンエラー
 * @retval RC_DB_FORMAT_ERROR     databaseファイルフォーマットエラー
 * @retval RC_MEM_ALLOCATE_ERROR  メモリ確保エラー
 * @retval RC_SYSTEM_ERROR        その他、致命的なエラー
 */
static int read_database(char *database, WordData *root)
{	
	WordData *new_word;

	/* open database */
	
	/* databaseを全て読み込み */
	while(0) {
		/* 単語を一つ取得 */
		new_word = make_new_word(NULL, 0);
		if(new_word == NULL) {}

		/* 取得した単語をリストに追加 */
		add_word(root, new_word);
	}

	/*close database */

	return 0;
}

/**
 * @brief WordDataリストの単語を、フォーマットに従いdatabaseに書き込む
 *
 * @param[in] database  書き込み対象のdatabase
 * @param[in] WordData  WordDataリストの先頭要素のアドレス
 *
 * @retval RC_NORMAL_END       正常終了
 * @retval RC_FILE_OPEN_ERROR  ファイルオープンエラー
 * @retval RC_SYSTEM_ERROR     その他、致命的なエラー
 */
static int write_database(char *database, WordData *root)
{
	return 0;
}

/**
 * @brief 入力ファイルの単語をカウントし、WordDataのリストに格納する
 *
 * @param[in]	  filename  読み込み対象の入力ファイル名
 * @param[in,out] root	    WordDataリストの先頭要素のアドレス
 *
 * @retval RC_NORMAL_END          正常終了
 * @retval RC_FILE_OPEN_ERROR     ファイルオープンエラー
 * @retval RC_INFILE_FORMAT_ERROR 入力ファイルフォーマットエラー
 * @retval RC_MEM_ALLOCATE_ERROR  メモリ確保エラー
 * @retval RC_SYSTEM_ERROR        その他、致命的なエラー
 */
static int read_input_file(char *filename, WordData *root)
{
	WordData *new_word;
	char *buff;
	/* open infile */
	
	/* 入力ファイルの全ての単語を読み込み */
	while(0) {
		/* 1単語読み込み */
		new_word = make_new_word(buff, 1);
		if(new_word == NULL) {}

		/* リストに追加 */
		add_word(new_word, root);
	}

	/* close infile */

	return 0;
}

/**
 * @brief 新しいWordDataを作り、そのポインタを返す
 *
 * @param[in] word   WordDataのメンバwordに格納する文字列
 * @param[in] count  WordDataのメンバcountに格納する数値
 *
 * @return 成功時：作成したWordData構造体のアドレス
 *         失敗時：NULL
 */
static WordData* make_new_word(char *word, int count)
{
	return NULL;
}

/**
 * @brief WordDataリストの中身をフォーマットに従い標準出力で表示する
 *
 * @param[in] root   WordDataリストの先頭要素のアドレス
 */
static void disp_all_words(WordData *root)
{
	return;
}

/**
 * @brief WordDataリストの要素用に確保したメモリを解放
 *
 * @param[in]   対象のリストの先頭要素のアドレス
 */
static void free_list(WordData *root)
{
	return;
}

/**
 * @brief WordDataリストに新しい単語を追加
 * 
 * @param[in]    new_word  追加対象のWordDataのアドレス
 * @param[in,out] root      new_word追加先のWordDataリストの先頭要素アドレス
 */
static void add_word(WordData *new_word, WordData *root)
{
	/* 単語がリストにあればcountを足す -> free(new_word) */
	/* 単語がリストになければWordData.nextを繋ぎ変える */
	return;
}

