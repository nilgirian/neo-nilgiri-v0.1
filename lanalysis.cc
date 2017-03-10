#include<curses.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>
#include<errno.h>
#include<assert.h>
#include<string.h>
#include"sqlite.h"

#define DEBUG(x)

#define LOGDB_FILE "logdB"
#define MAX_LOGLINE_SIZE 1024

sqlite *logdb;
time_t now=0;

struct _leaf;

union INDEXPTR
{
	int index;
	struct _leaf *ptr;
};

typedef struct _leaf
{
	char *token;
	char collapsed;
	char line;
	INDEXPTR l, r, next;
}
LEAF;

LEAF *tree;
int tree_top = -1;
int max_leaves = 0;
int top_of_tree_index = -1;

static void finish(int sig)
{
	endwin();
	sqlite_close(logdb);
	exit(0);
}

/** \return the index of the new leaf */
int add_to_tree(char *token, int parent_index)
{
	LEAF *leaf;
	char *p, *q;
	int leaf_index;

	// who's your daddy?
	if (parent_index == -1)
	{
		leaf_index = top_of_tree_index;
	}
	else
	{
		leaf_index = tree[parent_index].r.index;
	}

	// find the last token in the path
	if (leaf_index != -1)
	{
		while (1)
		{
			if (strcmp(token, tree[leaf_index].token)==0)
			{
				return leaf_index;
			}
			if (tree[leaf_index].next.index == -1)
			{
				break;
			}
			leaf_index = tree[leaf_index].next.index;
		}
	}

	if (tree_top == max_leaves-1)
	{
		if (max_leaves == 0)
		{
			max_leaves = 1;
		}
		else
		{
			max_leaves <<= 1;
		}
		if ((tree = (LEAF *) realloc(tree, sizeof(LEAF) * max_leaves))==NULL)
		{
			perror("realloc:");
			abort();
		}
	}
	tree_top++;

	tree[tree_top].token = token;
	tree[tree_top].l.index = parent_index;
	tree[tree_top].r.index = -1;
	tree[tree_top].next.index = -1;
	tree[tree_top].collapsed = 1;

	if (leaf_index != -1)
	{
		// we have a sibling
		tree[leaf_index].next.index = tree_top;
	}
	else if (parent_index != -1)
	{
		// we have no sibling but we have a parent
		tree[parent_index].r.index = tree_top;
	}
	else
	{
		// we are an orphan with no sibling
		top_of_tree_index = tree_top;
	}
	return tree_top;
}

/** Chops up a given path into tokens and uses the tokens for
    add_to_tree().  The new leaf is kept as the parent of the next leaf
	 for a given path.
 */
void add_path(char *path)
{
	char *p;
	char *token;
	int parent_index = -1;

	p = path;

	if (*p == '/')
	{
		token = p+1;
	}
	else
	{
		token = p;
	}
	p++;

	while (*p)
	{
		if (*p == '/')
		{
			*p = '\0';
			parent_index = add_to_tree(token, parent_index);
			token = p+1;
		}
		p++;
	}
	parent_index = add_to_tree(token, parent_index);
}

void assign_leaf_pointers(void)
{
	for (int i = 0; i <= tree_top; i++)
	{
		if (tree[i].l.index == -1)
		{
			tree[i].l.ptr = NULL;
		}
		else
		{
			tree[i].l.ptr = &tree[tree[i].l.index];
		}

		if (tree[i].r.index == -1)
		{
			tree[i].r.ptr = NULL;
		}
		else
		{
			tree[i].r.ptr = &tree[tree[i].r.index];
		}

		if (tree[i].next.index == -1)
		{
			tree[i].next.ptr = NULL;
		}
		else
		{
			tree[i].next.ptr = &tree[tree[i].next.index];
		}
	}
}

void tree_dump(void)
{
	fprintf(stderr, "TREE DUMP\r\n[token][next][left][right]\r\n");
	for (int i =0; i <= tree_top; i++)
	{
		fprintf(stderr, "[%s][%s][%s][%s]\n\r",
				tree[i].token,
				tree[i].next.ptr ? tree[i].next.ptr->token : "(null)",
				tree[i].l.ptr ? tree[i].l.ptr->token : "(null)",
				tree[i].r.ptr ? tree[i].r.ptr->token : "(null)");
	}
}

const char *timestr(time_t t0)
{
	static char buf[64];
	buf[0] = '\0';
	struct tm *tm0;
	tm0 = localtime(&t0);
	strftime(buf, 63, "%Y-%m-%e %I:%M:%S", tm0);
	return buf;
}

int callback(void *pArg, int argc, char **argv, char **columnName)
{
	unsigned short int session;
	time_t time;
	const char *line;
	char *path;

	assert(argc == 3);

	session = atoi(argv[0]);
	time = atol(argv[1]);
	line = argv[2];

	if ((path = strdup(line))==NULL)
	{
		perror("strdup:");
		abort();
	}
	DEBUG(fprintf(stderr, "PATH: %s\n\r", path));
	add_path(path);
	return 0;
}

void load(const char *condition="", time_t beg=0, time_t fin=now)
{
	char *dbErrMsg;
	char *buf[512];

	if (sqlite_exec_printf(logdb,
				"SELECT * FROM log %q ORDER BY line, session, time",
				callback,
				NULL,
				&dbErrMsg,
				condition) != SQLITE_OK)
	{
		fprintf(stderr, "%s: %s", dbErrMsg, strerror(errno));
		exit(0);
	}
	assign_leaf_pointers();
	DEBUG(tree_dump());
}

int offset = 0;
int mline = -1;
int collapse = -1;

void clicked(int cline)
{
	collapse = offset+cline;
}

const char *display_token(const char *token)
{
	static char buf[1024];
	const char *p;
	char *q;
	int s;

	p = token;
	q = buf;
	s = 0;

	while (*p)
	{
		if (s == 0)
		{
			if (*p == ':')
			{
				s = 1;
			}
			else
			{
				*q = *p;
				q++;
			}
		}
		else if (s == 1)
		{
			if (*p == ':')
			{
				s = 2;
			}
			else
			{
				s = 0;
				*q = ':';
				q++;
				*q = *p;
				q++;
			}
		}
		else if (s == 2)
		{
			if (*p == ':')
			{
				s = 0;
			}
		}
		p++;
	}
	*q = '\0';
	return buf;
}

void drawscreen(void)
{
	LEAF *leaf;
	int line=0, cx, cy = -1;
	int lvls;
	for (int i = 0; i <= tree_top; i++)
	{
		lvls = 0;
		for (leaf = tree[i].l.ptr; leaf; leaf = leaf->l.ptr)
		{
			if (leaf->collapsed)
			{
				break;
			}
			lvls++;
		}
		if (leaf) continue;
		if (++line < offset || cy >= LINES-2)
		{
			continue;
		}
		cx = lvls * 3;
		cy++;
		if (mline == cy && tree[i].r.ptr)
		{
			tree[i].collapsed = !tree[i].collapsed;
		}
		mvprintw(cy, cx, "%c %s",
				tree[i].r.ptr ? tree[i].collapsed ? '+' : '-' : ' ',
				display_token(tree[i].token));
	}
	mvprintw(LINES-1, 0, "offset:%d mline:%d i:%d", offset, mline, collapse);
	collapse = -1;
}

int main(int argc, char *argv[])
{
	char *dbErrMsg;
	mline = -1;
	now = time(0);

	if ((logdb = sqlite_open(LOGDB_FILE, 0, &dbErrMsg))==NULL)
	{
		fprintf(stderr, "/sqlite_open/%s", dbErrMsg);
		exit(0);
	}
	load();

	DEBUG(exit(0));

	MEVENT event;
	signal(SIGINT, finish); // arrange interrupts to terminate
	initscr();
	keypad(stdscr, TRUE);
	nonl();
	cbreak();
	noecho();
	curs_set(0);

	mousemask(ALL_MOUSE_EVENTS, NULL);

	int c;
	while (1)
	{
		erase();
		drawscreen();
		c = getch();
		mline = -1;
		switch (c)
		{
			case KEY_UP:
			case KEY_DOWN:
			case KEY_LEFT:
				printw("key left\n");
				break;
			case KEY_RIGHT:
				printw("key right\n");
				break;
			case KEY_NPAGE:
				if (offset < tree_top-5)
				{
					offset += 5;
				}
				else
				{
					offset = tree_top;
				}
				break;
			case KEY_PPAGE:
				if (offset > 5)
				{
					offset -= 5;
				}
				else
				{
					offset = 0;
				}
				break;
			case KEY_MOUSE:
				if (getmouse(&event)==OK)
				{
					if (event.bstate & BUTTON1_PRESSED)
					{
						mline = event.y;
					}
					if (event.bstate & BUTTON1_RELEASED)
					{
						if (event.y == mline)
						{
							clicked(mline);
						}
					}
					else if (event.bstate
							& (BUTTON1_CLICKED
								| BUTTON1_DOUBLE_CLICKED
								| BUTTON1_TRIPLE_CLICKED))
					{
						mline = event.y;
						clicked(mline);
					}
				}
				break;
			default:
				addch(c);
		}
		refresh();
	}
	finish(0);
	return 0;
}
