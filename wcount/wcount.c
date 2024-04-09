#include <stdio.h>

/*
 * 〇最終課題
 * ファイルに含まれる単語と出現回数を表示
 */

#define MODE_INPUT 1
#define MODE_DISP  2
#define RC_NORMAL_END		0
#define RC_ERR_ARG 		1
#define RC_ERR_FILE_OPEN 	2
#define RC_ERR_INFILE_FORMAT 	3
#define RC_ERR_DB_FORMAT	4
#define RC_ERR_MEM_ALLOCATE	5
#define RC_ERR_SYSTEM		6

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

/* proc_input_request()で使用する(初期化が必要な)データをまとめた構造体 */
typedef struct InputModeData_t {
	
} InputModeData;

/* proc_disp_request()で使用する(初期化が必要な)データをまとめた構造体 */
typedef struct DispModeData_t {

}  DispModeData;

static int analyze_args(int argc, char *argv[], CmdParams *params);
/* 入力モード時処理 */
static int proc_input_request(CmdParams params);
/* DB表示モード時処理 */
static int proc_disp_request(CmdParams params);
/* WordDataリスト操作関連 */
static int load_WDList_from_database(char *database, WordData *root);
static int save_WDList_to_database(char *database, WordData *root);
static int make_WDList_from_infile(char *infile, WordData *root);
static WordData* create_WD(char *word, int count);
static WordData* init_WDList();
static void disp_WDList(WordData *root);
static void free_WDList(WordData *root);
static void add_to_WDList(WordData *new_word, WordData *root);

int main(int argc, char *argv[])
{
	int rc;
	CmdParams params;

	/* 引数解析 */
	rc = analyze_args(argc, argv, &params);
	if(rc == -1){}

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
	return 0;
}

/**
 * @brief 入力モード時の処理
 *
 * @param[in] params コマンドライン引数の解析済データ
 *
 * @retval RC_NORMAL_END          正常終了
 * @retval RC_ERR_FILE_OPEN	  ファイルオープンエラー
 * @retval RC_ERR_INFILE_FORMAT 入力ファイルフォーマットエラー
 * @retval RC_ERR_DB_FORMAT     databaseファイルフォーマットエラー
 * @retval RC_ERR_MEM_ALLOCATE  メモリ確保エラー
 * @retval RC_ERR_SYSTEM        その他、致命的なエラー
 */
static int proc_input_request(CmdParams params)
{
	int rc = RC_NORMAL_END;
	WordData *root;
	root = init_WDList();
	if(root == NULL) {}

	/* read database */
	if(params.db_flag) {
		rc = load_WDList_from_database(params.database, root);
	}
	if(rc != RC_NORMAL_END) {}

	/* read infile */
	rc = make_WDList_from_infile(params.infile, root);
	if(rc != RC_NORMAL_END) {}

	/* output data */
	if(params.db_flag) {
		/* write database */
		rc = save_WDList_to_database(params.database, root);
		if(rc != RC_NORMAL_END) {}
	} else {
		/* disp all words(stdout) */
		disp_WDList(root);
	}

	/* clear list */
	free_WDList(root);

	return 0;
}

/**
 * @brief DB表示モード時の処理
 *
 * @param[in] params コマンドライン引数の解析済データ
 *
 * @retval RC_NORMAL_END	  正常終了
 * @retval RC_ERR_FILE_OPEN     ファイルオープンエラー
 * @retval RC_ERR_DB_FORMAT	  databaseファイルフォーマットエラー
 * @retval RC_ERR_MEM_ALLOCATE  メモリ確保エラー
 * @retval RC_ERR_SYSTEM	  その他、致命的なエラー
 */
static int proc_disp_request(CmdParams params)
{
	int rc = RC_NORMAL_END;
	WordData *root;
	root = init_WDList();
	if(root == NULL) {}
	
	/* read database */
	rc = load_WDList_from_database(params.database, root);
	if(rc != RC_NORMAL_END) {}

	/* disp all words(stdout) */
	disp_WDList(root);
	
	/* リストのメモリを解放 */
	free_WDList(root);

	return 0;
}

/**
 * @brief databaseから単語データを読み込んでWordDataのリストに格納する
 * 
 * @param[in]	  database  読み込み対象のdatabase
 * @param[in,out] root      WordDataリストの先頭要素のアドレス
 *
 * @retval RC_NORMAL_END          正常終了
 * @retval RC_ERR_FILE_OPEN     ファイルオープンエラー
 * @retval RC_ERR_DB_FORMAT     databaseファイルフォーマットエラー
 * @retval RC_ERR_MEM_ALLOCATE  メモリ確保エラー
 * @retval RC_ERR_SYSTEM        その他、致命的なエラー
 */
static int load_WDList_from_database(char *database, WordData *root)
{	
	WordData *new_word;

	/* open database */
	
	/* databaseを全て読み込み */
	while(0) {
		/* 単語を一つ取得 */
		new_word = create_WD(NULL, 0);
		if(new_word == NULL) {}

		/* 取得した単語をリストに追加 */
		add_to_WDList(root, new_word);
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
 * @retval RC_ERR_FILE_OPEN  ファイルオープンエラー
 * @retval RC_ERR_SYSTEM     その他、致命的なエラー
 */
static int save_WDList_to_database(char *database, WordData *root)
{
	return 0;
}

/**
 * @brief 入力ファイルの単語をカウントし、WordDataのリストに格納する
 *
 * @param[in]	  infile  読み込み対象の入力ファイル名
 * @param[in,out] root	  WordDataリストの先頭要素のアドレス
 *
 * @retval RC_NORMAL_END          正常終了
 * @retval RC_ERR_FILE_OPEN     ファイルオープンエラー
 * @retval RC_ERR_INFILE_FORMAT 入力ファイルフォーマットエラー
 * @retval RC_ERR_MEM_ALLOCATE  メモリ確保エラー
 * @retval RC_ERR_SYSTEM        その他、致命的なエラー
 */
static int make_WDList_from_infile(char *infile, WordData *root)
{
	WordData *new_word;
	char *buff;
	/* open infile */
	
	/* 入力ファイルの全ての単語を読み込み */
	while(0) {
		/* 1単語読み込み */
		new_word = create_WD(buff, 1);
		if(new_word == NULL) {}

		/* リストに追加 */
		add_to_WDList(new_word, root);
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
static WordData* create_WD(char *word, int count)
{
	return NULL;
}

/**
 * @brief 新しいWordDataのリストを作成し、先頭要素のアドレスを返す
 */
static WordData* init_WDList()
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
	return;
}

/**
 * @brief WordDataリストの要素用に確保したメモリを解放
 *
 * @param[in]   対象のリストの先頭要素のアドレス
 */
static void free_WDList(WordData *root)
{
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
	/* 単語がリストにあればcountを足す -> free(new_word) */
	/* 単語がリストになければWordData.nextを繋ぎ変える */
	return;
}

