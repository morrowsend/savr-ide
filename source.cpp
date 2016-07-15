 //
// This program was written in Quincy 2005.
// It was modified from the FLTK example text editor program.
// Basically, all I did was change the syntax highlighting and 
// add the "assemble" command. This program uses AVRA.exe.
// I'm a beginner at FLTK so I didn't do a lot. I'm still learning... 
// I attempted a few things, (such as the popen in another window stuff)
// which I have since commented out because they didn't work corectly.
// Feel free to throw down on the code.
// If you do make it better, however, please let me see the finished
// results and code.
// For now, this program is hosted and an updated version is likely
// at my business site: http://www.sheekgeek.com
// Have fun!
//
//--Adam

// "$Id: editor.cxx 4723 2005-12-30 10:13:17Z matt $"
//
//
// Copyright 1998-2005 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

//
// Include necessary headers...
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifdef __MWERKS__
# define FL_DLL 
#endif

/*#ifdef _WIN32
#include <windows.h>
#define PING_CMD ("avra.exe %s ", title)       //<-- that's not gonna work, of course...
  
#define popen  _popen
#define pclose _pclose
#else
#include <unistd.h>
//#define PING_CMD "ping localhost"        // 'slow command' under unix
#endif
*/

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Help_Dialog.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Multi_Browser.H>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Tooltip.H>
#include <FL/Fl_Input_Choice.H>
#include <fstream>
#include <iostream>
#include <string>
using std::string;
using namespace std;

bool				asm_flag=false;
int                changed = 0;
char               filename[350] = "";
char               filename2[350] = "";
char               title[256];
char               name[256];
 char buffer[150];
 char buffer2[350];
 char buffer3[15];
 char buffer4[15];
 char buffer5[15];
 char buffer6[100];
 char buffer7[100];
 char buffer8[100];
 bool write=false;
 bool read=false;
 bool verify=false;
 bool wf=false;
 bool rf=false;
 bool vf=false;
 bool cf=true;
 bool prf=true;
 bool pof=true;
 int intValue;
 bool just_opened=true;
//char buffer9[300];


Fl_Text_Buffer     *textbuf = 0;
FILE *G_fp = NULL;
    // Popen stuff
    FILE *fp;
    char command[512];

    // File editor
    Fl_Text_Editor *file_edit;
    Fl_Text_Buffer *file_buff;

    // Console editor
    Fl_Text_Editor *console_edit;
    Fl_Text_Buffer *console_buff;

// Syntax highlighting stuff...
Fl_Text_Buffer     *stylebuf = 0;
Fl_Text_Display::Style_Table_Entry
                   styletable[] = {	// Style table
		     { FL_BLACK,      FL_COURIER,        14 }, // A - Plain
		     { FL_DARK_GREEN, FL_COURIER_ITALIC, 14 }, // B - Line comments
		     { FL_DARK_GREEN, FL_COURIER_ITALIC, 14 }, // C - Block comments
		     { FL_BLUE,       FL_COURIER,        14 }, // D - Strings
		     { FL_DARK_RED,   FL_COURIER,        14 }, // E - Directives
		     { FL_DARK_RED,   FL_COURIER_BOLD,   14 }, // F - Types
		     { FL_BLUE,       FL_COURIER_BOLD,   14 }  // G - Keywords
		   };
const char         *code_keywords[] = {	// List of known C/C++ keywords...
			"define",
			 "equ"
		   };
const char         *code_types[] = {	// List of known C/C++ types...
		     "adc",
			"add",
			"adiw",
			"and",
			"andi",
			"asr",
			"bclr",
			"bld",
			"brbc",
			"brbs",
			"brcc",
			"brcs",
			"break",
			"breq",
			"brge",
			"brhc",
			"brhs",
			"brid",
			"brie",
			"brlo",
			"brlt",
			"brmi",
			"brne",
			"brpl",
			"brsh",
			"brtc",
			"brts",
			"brvc",
			"brvs",
			"bset",
			"bst",
			"cbi",
			"cbr",
			"clc",
			"clh",
			"cli",
			"cln",
			"cln",
			"clr",
			"cls",
			"clt",
			"clv",
			"clz",
			"com",
			"cp",
			"cpc",
			"cpi",
			"cpse",	
			"dec",
			"eor",
			"icall",
			"ijmp",
			"in",
			"inc",
			"ld",
			"ldd",
			"ldi",
			"lpm",
			"lsl",
			"lsr",
			"mov",
			"movw",
			"neg",
			"nop",
			"or",
			"ori",
			"out",
			"pop",
			"push",
			"rcall",
			"ret",
			"reti",
			"rjmp",
			"rol",
			"ror",
			"sbc",
			"sbci",
   	    	"sbi",
			"sbic",
			"sbis",
			"sbiw",
			"sbr",	
			"sbrc",
			"sbrs",
			"sec",
			"seh",
			"sei",
			"sen",
			"ser",
			"ses",
			"set",
			"sev",
			"sez",
			"sleep",
			"spm",
			"st",
			"std",
			"sts",
			"sub",
			"subi",
			"swap",
			"tst",
			"wdr",    
		};


//
// 'compare_keywords()' - Compare two keywords...
//

extern "C" {
  int
  compare_keywords(const void *a,
                   const void *b) {
    return (strcmp(*((const char **)a), *((const char **)b)));
  }
}

//
// 'style_parse()' - Parse text and produce style data.
//

void
style_parse(const char *text,
            char       *style,
	    int        length) {
  char	     current;
  int	     col;
  int	     last;
  char	     buf[255],
             *bufptr;
  const char *temp;

  // Style letters:
  //
  // A - Plain
  // B - Line comments
  // C - Block comments
  // D - Strings
  // E - Directives
  // F - Types
  // G - Keywords

  for (current = *style, col = 0, last = 0; length > 0; length --, text ++) {
    if (current == 'B' || current == 'F' || current == 'G') current = 'A';
    if (current == 'A') {
      // Check for directives, comments, strings, and keywords...
      if (col == 0 && *text == '#') {
        // Set style to directive
        current = 'E';
      } else if (strncmp(text, "//", 2) == 0 || *text == ';') {
        current = 'B';
	for (; length > 0 && *text != '\n'; length --, text ++) *style++ = 'B';

        if (length == 0) break;
      } else if (strncmp(text, "/*", 2) == 0) {
        current = 'C';
      } else if (strncmp(text, "\\\"", 2) == 0) {
        // Quoted quote...
	*style++ = current;
	*style++ = current;
	text ++;
	length --;
	col += 2;
	continue;
      } else if (*text == '\"') {
        current = 'D';
      } else if (!last && (islower(*text) || *text == '_')) {
        // Might be a keyword...
	for (temp = text, bufptr = buf;
	     (islower(*temp) || *temp == '_') && bufptr < (buf + sizeof(buf) - 1);
	     *bufptr++ = *temp++);

        if (!islower(*temp) && *temp != '_') {
	  *bufptr = '\0';

          bufptr = buf;

	  if (bsearch(&bufptr, code_types,
	              sizeof(code_types) / sizeof(code_types[0]),
		      sizeof(code_types[0]), compare_keywords)) {
	    while (text < temp) {
	      *style++ = 'F';
	      text ++;
	      length --;
	      col ++;
	    }

	    text --;
	    length ++;
	    last = 1;
	    continue;
	  } else if (bsearch(&bufptr, code_keywords,
	                     sizeof(code_keywords) / sizeof(code_keywords[0]),
		             sizeof(code_keywords[0]), compare_keywords)) {
	    while (text < temp) {
	      *style++ = 'G';
	      text ++;
	      length --;
	      col ++;
	    }

	    text --;
	    length ++;
	    last = 1;
	    continue;
	  }
	}
      }
    } else if (current == 'C' && strncmp(text, "*/", 2) == 0) {
      // Close a C comment...
      *style++ = current;
      *style++ = current;
      text ++;
      length --;
      current = 'A';
      col += 2;
      continue;
    } else if (current == 'D') {
      // Continuing in string...
      if (strncmp(text, "\\\"", 2) == 0) {
        // Quoted end quote...
	*style++ = current;
	*style++ = current;
	text ++;
	length --;
	col += 2;
	continue;
      } else if (*text == '\"') {
        // End quote...
	*style++ = current;
	col ++;
	current = 'A';
	continue;
      }
    }

    // Copy style info...
    if (current == 'A' && (*text == '{' || *text == '}')) *style++ = 'G';
    else *style++ = current;
    col ++;

    last = isalnum(*text) || *text == '_' || *text == '.';

    if (*text == '\n') {
      // Reset column and possibly reset the style
      col = 0;
      if (current == 'B' || current == 'E') current = 'A';
    }
  }
}


//
// 'style_init()' - Initialize the style buffer...
//

void
style_init(void) {
  char *style = new char[textbuf->length() + 1];
  char *text = textbuf->text();
  

  memset(style, 'A', textbuf->length());
  style[textbuf->length()] = '\0';

  if (!stylebuf) stylebuf = new Fl_Text_Buffer(textbuf->length());

  style_parse(text, style, textbuf->length());

  stylebuf->text(style);
  delete[] style;
  free(text);
}


//
// 'style_unfinished_cb()' - Update unfinished styles.
//

void
style_unfinished_cb(int, void*) {
}


//
// 'style_update()' - Update the style buffer...
//

void
style_update(int        pos,		// I - Position of update
             int        nInserted,	// I - Number of inserted chars
	     int        nDeleted,	// I - Number of deleted chars
             int        /*nRestyled*/,	// I - Number of restyled chars
	     const char * /*deletedText*/,// I - Text that was deleted
             void       *cbArg) {	// I - Callback data
  int	start,				// Start of text
	end;				// End of text
  char	last,				// Last style on line
	*style,				// Style data
	*text;				// Text data


  // If this is just a selection change, just unselect the style buffer...
  if (nInserted == 0 && nDeleted == 0) {
    stylebuf->unselect();
    return;
  }

  // Track changes in the text buffer...
  if (nInserted > 0) {
    // Insert characters into the style buffer...
    style = new char[nInserted + 1];
    memset(style, 'A', nInserted);
    style[nInserted] = '\0';

    stylebuf->replace(pos, pos + nDeleted, style);
    delete[] style;
  } else {
    // Just delete characters in the style buffer...
    stylebuf->remove(pos, pos + nDeleted);
  }

  // Select the area that was just updated to avoid unnecessary
  // callbacks...
  stylebuf->select(pos, pos + nInserted - nDeleted);

  // Re-parse the changed region; we do this by parsing from the
  // beginning of the previous line of the changed region to the end of
  // the line of the changed region...  Then we check the last
  // style character and keep updating if we have a multi-line
  // comment character...
  start = textbuf->line_start(pos);
//  if (start > 0) start = textbuf->line_start(start - 1);
  end   = textbuf->line_end(pos + nInserted);
  text  = textbuf->text_range(start, end);
  style = stylebuf->text_range(start, end);
  if (start==end)
    last = 0;
  else
    last  = style[end - start - 1];

//  printf("start = %d, end = %d, text = \"%s\", style = \"%s\", last='%c'...\n",
//         start, end, text, style, last);

  style_parse(text, style, end - start);

//  printf("new style = \"%s\", new last='%c'...\n", 
//         style, style[end - start - 1]);

  stylebuf->replace(start, end, style);
  ((Fl_Text_Editor *)cbArg)->redisplay_range(start, end);

  if (start==end || last != style[end - start - 1]) {
//    printf("Recalculate the rest of the buffer style\n");
    // Either the user deleted some text, or the last character 
    // on the line changed styles, so reparse the
    // remainder of the buffer...
    free(text);
    free(style);

    end   = textbuf->length();
    text  = textbuf->text_range(start, end);
    style = stylebuf->text_range(start, end);

    style_parse(text, style, end - start);

    stylebuf->replace(start, end, style);
    ((Fl_Text_Editor *)cbArg)->redisplay_range(start, end);
  }

  free(text);
  free(style);
}


// Editor window functions and class...
void save_cb();
void saveas_cb();
void find2_cb(Fl_Widget*, void*);
void replall_cb(Fl_Widget*, void*);
void replace2_cb(Fl_Widget*, void*);
void replcan_cb(Fl_Widget*, void*);

//void HandleFD(int fd, void *data);


class EditorWindow : public Fl_Double_Window {
  public:
    EditorWindow(int w, int h, const char* t);
    ~EditorWindow();

    Fl_Window          *replace_dlg;
    Fl_Input           *replace_find;
    Fl_Input           *replace_with;
    Fl_Button          *replace_all;
    Fl_Return_Button   *replace_next;
    Fl_Button          *replace_cancel;

    Fl_Text_Editor     *editor;
    char               search[256];
};








EditorWindow::EditorWindow(int w, int h, const char* t) : Fl_Double_Window(w, h, t) {
  replace_dlg = new Fl_Window(300, 105, "Replace");
    replace_find = new Fl_Input(80, 10, 210, 25, "Find:");
    replace_find->align(FL_ALIGN_LEFT);

    replace_with = new Fl_Input(80, 40, 210, 25, "Replace:");
    replace_with->align(FL_ALIGN_LEFT);

    replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
    replace_all->callback((Fl_Callback *)replall_cb, this);

    replace_next = new Fl_Return_Button(105, 70, 120, 25, "Replace Next");
    replace_next->callback((Fl_Callback *)replace2_cb, this);

    replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");
    replace_cancel->callback((Fl_Callback *)replcan_cb, this);
  replace_dlg->end();
  replace_dlg->set_non_modal();
  editor = 0;
  *search = (char)0;
}

EditorWindow::~EditorWindow() {
  delete replace_dlg;
}

int check_save(void) {
  if (!changed) return 1;

  int r = fl_choice("The current file has not been saved.\n"
                    "Would you like to save it now?",
                    "Cancel", "Save", "Don't Save");

  if (r == 1) {
    save_cb(); // Save the file...
    return !changed;
  }

  return (r == 2) ? 1 : 0;
}

int loading = 0;
void load_file(char *newfile, int ipos) {
  loading = 1;
  int insert = (ipos != -1);
  changed = insert;
  if (!insert) strcpy(filename, "");
  int r;
  if (!insert) r = textbuf->loadfile(newfile);
  else r = textbuf->insertfile(newfile, ipos);
  if (r)
    fl_alert("Error reading from file \'%s\':\n%s.", newfile, strerror(errno));
  else
    if (!insert) strcpy(filename, newfile);
  loading = 0;
  textbuf->call_modify_callbacks();
}

void save_file(char *newfile) {
  if (textbuf->savefile(newfile))
    fl_alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
  else
    strcpy(filename, newfile);
  changed = 0;
  textbuf->call_modify_callbacks();
}

void copy_cb(Fl_Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  Fl_Text_Editor::kf_copy(0, e->editor);
}

void cut_cb(Fl_Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  Fl_Text_Editor::kf_cut(0, e->editor);
}

void delete_cb(Fl_Widget*, void*) {
  textbuf->remove_selection();
}

void find_cb(Fl_Widget* w, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  const char *val;

  val = fl_input("Search String:", e->search);
  if (val != NULL) {
    // User entered a string - go find it!
    strcpy(e->search, val);
    find2_cb(w, v);
  }
}

void find2_cb(Fl_Widget* w, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  if (e->search[0] == '\0') {
    // Search string is blank; get a new one...
    find_cb(w, v);
    return;
  }

  int pos = e->editor->insert_position();
  int found = textbuf->search_forward(pos, e->search, &pos);
  if (found) {
    // Found a match; select and update the position...
    textbuf->select(pos, pos+strlen(e->search));
    e->editor->insert_position(pos+strlen(e->search));
    e->editor->show_insert_position();
  }
  else fl_alert("No occurrences of \'%s\' found!", e->search);
}

void set_title(Fl_Window* w) {
  if (filename[0] == '\0') strcpy(title, "Untitled");
  else {
    char *slash;
    slash = strrchr(filename, '/');
#ifdef WIN32
    if (slash == NULL) slash = strrchr(filename, '\\');
#endif
    if (slash != NULL) strcpy(title, slash + 1);
    else strcpy(title, filename);
   char *dot;
dot = strchr(filename, '.');
strncpy (name,filename,dot-filename+1);  
strcat (name,"hex");
  }

  if (changed) strcat(title, " (modified)");

  w->label(title);


}

void changed_cb(int, int nInserted, int nDeleted,int, const char*, void* v) {
  if ((nInserted || nDeleted) && !loading) changed = 1;
  EditorWindow *w = (EditorWindow *)v;
  set_title(w);
  if (loading) w->editor->show_insert_position();
}

void input_choice_cb(Fl_Widget*,void*data) {
    Fl_Input_Choice *in=(Fl_Input_Choice *)data;
    fprintf(stderr, "Value='%s'\n", (const char*)in->value());
}

void about_cb(Fl_Widget*, void*) 
{ //fl_choice("Based on FLTK example text editor program,\nusing AVRA.exe assembler. \n\n\t\t\tBy: Adam Harris");
fl_choice("\t\t\tSAVR IDE V0.2\n\n\tBased on FLTK example text editor program,\n\tusing AVRA.exe assembler and \n\tAVRdude.exe uploader/downloader \n\n\t\t\tBy: Adam Harris", 0L, "OK", 0L, "OK");
}

void help_cb(Fl_Widget*, void*)
{

system("notepad README");
}
  





 void HandleFD() {
        static char s[1024];
        if ( fgets(s, sizeof(s)-1, fp) == NULL ) {
            Fl::remove_fd(fileno(fp));
            pclose(fp);
            fp = 0;
            console_buff->append("--- COMMAND COMPLETED\n");
        } else {
            console_buff->append(s);
        }
    }
    static void HandleFD_CB(int, void *userdata) {
        EditorWindow *w = (EditorWindow*)userdata;
        HandleFD();                               // avoids having to do ewin->xxx to access vars
    }
	    void RunCommand() {
        if ( fp != 0 ) return;                          // command already running? ignore
        console_buff->text("");                         // clear window
        if ( buffer[0] == 0 ) {                        // no command? complain
            console_buff->text("No command specified");
            return;
        }
        fp = popen(buffer, "r");                       // start command running..
        if ( fp == NULL ) {                             // Failed? show error
            console_buff->append("ERROR: popen: ");
            console_buff->append(strerror(errno));
        } else {                                        // Worked? setup handler
          // Fl::add_fd(fileno(G_fp), HandleFD, (void*)&brow);
		    Fl::add_fd(fileno(fp), HandleFD_CB);//, (void*));
        }
    }
	void RunCommand2() {
        if ( fp != 0 ) return;                          // command already running? ignore
        console_buff->text("");                         // clear window
        if ( buffer2[0] == 0 ) {                        // no command? complain
            console_buff->text("No command specified");
            return;
        }
        fp = popen(buffer2, "r");                       // start command running..
        if ( fp == NULL ) {                             // Failed? show error
            console_buff->append("ERROR: popen: ");
            console_buff->append(strerror(errno));
        } else {                                        // Worked? setup handler
          // Fl::add_fd(fileno(G_fp), HandleFD, (void*)&brow);
		    Fl::add_fd(fileno(fp), HandleFD_CB);//, (void*));
        }
    }
    // MENU CALLBACK: Build command
    static void MakeCommand_CB(Fl_Widget*, void*userdata) {
        EditorWindow *ewin = (EditorWindow*)userdata;
        //strcpy(command, "netstat -an 2>&1");      // create the command
		// sprintf(buffer, "avra.exe \"%s\" 2>&1 ", filename);
		  sprintf(buffer2, "avrdude -p attiny2313 -P lpt1 -c dapa -E reset -U flash:w:\"%s\" 2>&1 ", filename);
		RunCommand2();
    }
    // MENU CALLBACK: Start command running

    // MENU CALLBACK: Start command running
    static void RunCommand_CB(Fl_Widget*, void*userdata) {
        EditorWindow *w = (EditorWindow*)userdata;
        RunCommand();                             // avoids having to do ewin->xxx to access vars
    }
	
	
	
	
	
	
	
	


    static void assemble_cb(Fl_Widget*, void*userdata) {
        EditorWindow *ewin = (EditorWindow*)userdata;
         if (changed && !check_save())
   return;
		     
		 sprintf(buffer, "bin\\avra.exe \"%s\" 2>&1 ", filename);// create the command
		 
		RunCommand();
    }


  
Fl_Choice *wChip=(Fl_Choice *)0;
  
  
void chip_cb(Fl_Widget*, void* userdata) {
//  printf("Chip selected '%s'\n", (const char*)userdata);

 //    printf("\n%d\n",intValue);
sprintf(buffer3, "-p %s", (const char*)userdata);
//printf(buffer3);
}
void programmer_cb(Fl_Widget*, void* userdata2) {
//  printf("Programmer selected '%s'\n", (const char*)userdata2);
 
sprintf(buffer4, "-c %s", (const char*)userdata2);
//printf(buffer4);
}
void port_cb(Fl_Widget*, void* userdata3) {
//  printf("Port selected '%s'\n", (const char*)userdata3);

sprintf(buffer5, "-P %s", (const char*)userdata3);
//  printf(buffer5);
}



void program_cb(Fl_Widget*, void*)
 {

  if (changed && !check_save())
   return;
    
if(buffer3[0]!='-')
{
  fl_alert("Please select a CHIP in 'Options'");
}
if(buffer4[0]!='-')
{
  fl_alert("Please select a PROGRAMMER in 'Options'");
}
if(buffer5[0]!='-')
{
  fl_alert("Please select a PORT in 'Options'");
}

else{

if (write==false && read==false && verify==false)
{
/*if (wf==true || rf ==true || vf==true)
{
fl_alert("Please doublecheck your programming options in the 'Options' window.");
}
else
{*/
fl_alert("Please choose a read, write or verify operation in 'Options'");
//}
}
if (write==false && read==false && verify==true)
{
sprintf(buffer2, "bin\\avrdude.exe  %s %s %s -E reset %s 2>&1", buffer3, buffer5, buffer4, buffer8);

}
if (write==false && read==true && verify==false)
{
sprintf(buffer2, "bin\\avrdude.exe  %s %s %s -E reset %s 2>&1", buffer3, buffer5, buffer4, buffer7);

}
if (write==false && read==true && verify==true)
{
sprintf(buffer2, "bin\\avrdude.exe  %s %s %s -E reset %s %s 2>&1", buffer3, buffer5, buffer4, buffer7, buffer8);

}
if (write==true && read==false && verify==false)
{
sprintf(buffer2,"bin\\avrdude.exe  %s %s %s -E reset %s 2>&1", buffer3, buffer5, buffer4, buffer6);

}
if (write==true && read==false && verify==true)
{
sprintf(buffer2, "bin\\avrdude.exe  %s %s %s -E reset %s %s 2>&1", buffer3, buffer5, buffer4, buffer6, buffer8);


}
if (write==true && read==true && verify==false)
{
sprintf(buffer2, "bin\\avrdude.exe  %s %s %s -E reset %s %s 2>&1", buffer3, buffer5, buffer4, buffer6, buffer7);

}
if (write==true && read==true && verify==true)
{
sprintf(buffer2, "bin\\avrdude.exe  %s %s %s -E reset %s %s %s 2>&1", buffer3, buffer5, buffer4, buffer6, buffer7, buffer8);

//system("cls");
//system("echo Simple AVR IDE");
//system("echo (Do not close this window)");

}

   
}

RunCommand2();
}

void write_cb() 
{
write=!write;
if (write==true || wf==true)
{
 sprintf(buffer6, "-U flash:w:\"%s\":i",name);
 wf=false;
}
else if (write==false)
{
sprintf(buffer6," ");
}
//printf(buffer6);
}

void read_cb() 
{
read=!read;
if (read==true || rf==true)
{
 sprintf(buffer7, "-U flash:r:\"%s\":i",name);
 rf=false;
}
else if (read==false)
{
sprintf(buffer7," ");
}
//printf(buffer7);
}

void verify_cb() 
{
verify=!verify;

if (verify==true || vf==true)
{
 sprintf(buffer8, "-U flash:v:\"%s\":i",name);
 vf=false;
}
else if (verify==false)
{
sprintf(buffer8," ");
}
//printf(buffer8);
}

void readPrefs();
void writePrefs();

Fl_Double_Window *myWindow=(Fl_Double_Window *)0;


Fl_Menu_Item menu_wChip[] = {
 {"Select One", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATtiny12", 0,  (Fl_Callback*)chip_cb, (void*)("t10"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATtiny13", 0,  (Fl_Callback*)chip_cb, (void*)("t13"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATtiny15", 0,  (Fl_Callback*)chip_cb, (void*)("t15"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATtiny26", 0,  (Fl_Callback*)chip_cb, (void*)("t26"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATtiny2313", 0,  (Fl_Callback*)chip_cb, (void*)("t2313"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega8", 0,  (Fl_Callback*)chip_cb, (void*)("m8"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega16", 0,  (Fl_Callback*)chip_cb, (void*)("m16"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega32", 0,  (Fl_Callback*)chip_cb, (void*)("m32"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega48", 0,  (Fl_Callback*)chip_cb, (void*)("m48"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega64", 0,  (Fl_Callback*)chip_cb, (void*)("m64"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega88", 0,  (Fl_Callback*)chip_cb, (void*)("m88"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega103", 0,  (Fl_Callback*)chip_cb, (void*)("m103"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega128", 0,  (Fl_Callback*)chip_cb, (void*)("m128"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega161", 0,  (Fl_Callback*)chip_cb, (void*)("m161"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega162", 0,  (Fl_Callback*)chip_cb, (void*)("m162"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega163", 0,  (Fl_Callback*)chip_cb, (void*)("m163"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega169", 0,  (Fl_Callback*)chip_cb, (void*)("m169"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega8515", 0,  (Fl_Callback*)chip_cb, (void*)("m8515"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ATmega8535", 0,  (Fl_Callback*)chip_cb, (void*)("m8535"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AT90S1200", 0,  (Fl_Callback*)chip_cb, (void*)("1200"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AT90S2313", 0,  (Fl_Callback*)chip_cb, (void*)("2313"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AT90S2333", 0,  (Fl_Callback*)chip_cb, (void*)("2333"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AT90S2343", 0,  (Fl_Callback*)chip_cb, (void*)("2343"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AT90S4414", 0,  (Fl_Callback*)chip_cb, (void*)("4414"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AT90S4433", 0,  (Fl_Callback*)chip_cb, (void*)("4433"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AT90S4434", 0,  (Fl_Callback*)chip_cb, (void*)("4434"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AT90S8515", 0,  (Fl_Callback*)chip_cb, (void*)("8515"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AT90S8535", 0,  (Fl_Callback*)chip_cb, (void*)("8535"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};


Fl_Check_Button *wWrite=(Fl_Check_Button *)0;

Fl_Check_Button *wRead=(Fl_Check_Button *)0;

Fl_Check_Button *wVerify=(Fl_Check_Button *)0;

Fl_Choice *wProgrammer=(Fl_Choice *)0;

Fl_Menu_Item menu_wProgrammer[] = {
 {"Select One", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"ABCmini", 0,  (Fl_Callback*)programmer_cb, (void*)("abcmini"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Alf", 0,  (Fl_Callback*)programmer_cb, (void*)("alf"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AVR910", 0,  (Fl_Callback*)programmer_cb, (void*)("avr910"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"AVRisp", 0,  (Fl_Callback*)programmer_cb, (void*)("avrisp"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Bascom", 0,  (Fl_Callback*)programmer_cb, (void*)("bascom"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"BSD", 0,  (Fl_Callback*)programmer_cb, (void*)("bsd"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Butterfly", 0,  (Fl_Callback*)programmer_cb, (void*)("butterfly"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Dragon", 0,  (Fl_Callback*)programmer_cb, (void*)("dragon"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"DAPA", 0,  (Fl_Callback*)programmer_cb, (void*)("dapa"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"DT006", 0,  (Fl_Callback*)programmer_cb, (void*)("dt006"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Futurlec", 0,  (Fl_Callback*)programmer_cb, (void*)("futurlec"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"PAVR", 0,  (Fl_Callback*)programmer_cb, (void*)("pavr"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Picoweb", 0,  (Fl_Callback*)programmer_cb, (void*)("picoweb"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Pony-STK200", 0,  (Fl_Callback*)programmer_cb, (void*)("pony-stk200"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"SP12", 0,  (Fl_Callback*)programmer_cb, (void*)("sp12"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"STK200", 0,  (Fl_Callback*)programmer_cb, (void*)("stk200"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"STK500", 0,  (Fl_Callback*)programmer_cb, (void*)("stk500"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Choice *wPort=(Fl_Choice *)0;

Fl_Menu_Item menu_wPort[] = {
  {"Select One", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
  {"lpt1", 0,  (Fl_Callback*)port_cb, (void*)("lpt1"), 0, FL_NORMAL_LABEL, 0, 14, 0},
  {"lpt2", 0,  (Fl_Callback*)port_cb, (void*)("lpt2"), 0, FL_NORMAL_LABEL, 0, 14, 0},
  {"lpt3", 0,  (Fl_Callback*)port_cb, (void*)("lpt3"), 0, FL_NORMAL_LABEL, 0, 14, 0},
  {"com1", 0,  (Fl_Callback*)port_cb, (void*)("com1"), 0, FL_NORMAL_LABEL, 0, 14, 0},
  {"com2", 0,  (Fl_Callback*)port_cb, (void*)("com2"), 0, FL_NORMAL_LABEL, 0, 14, 0},
  {"com3", 0,  (Fl_Callback*)port_cb, (void*)("com3"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

//static void closeWindowCB( Fl_Widget*, void* ) {
//  delete myWindow;
//}

static void saveAndCloseWindowCB( Fl_Widget*, void* ) {
just_opened=false;
  writePrefs();
 delete myWindow;
}




static void options_cb(int argc, char ** argv)
{ 
if (filename[1]!=':')
{
  fl_alert("Please open or name a file before setting options.");
  }
  else 
  {
   Fl_Double_Window* w;
  { Fl_Double_Window* o = myWindow = new Fl_Double_Window(300, 320, "AVRdude Options");
    w = o;
    o->callback((Fl_Callback*)saveAndCloseWindowCB);
    
	/*{ Fl_Button* o = new Fl_Button(210, 275, 75, 25, "Cancel");
      o->callback((Fl_Callback*)closeWindowCB);
    }*/
	
    { Fl_Button* o = new Fl_Button(210, 275, 75, 25, "OK");
      o->callback((Fl_Callback*)saveAndCloseWindowCB);
    }
    { Fl_Group* o = new Fl_Group(20, 30, 260, 100, "Flash:(Select only one)");
      o->box(FL_ENGRAVED_FRAME);
      o->align(FL_ALIGN_TOP_LEFT); 
     
      { Fl_Choice* o = wChip = new Fl_Choice(25, 90, 105, 25, "Chip:");
        o->down_box(FL_BORDER_BOX);
        o->align(FL_ALIGN_TOP_LEFT);
        o->menu(menu_wChip);
	//	  o->callback((Fl_Callback*)beepcb);
      }
      { Fl_Group* o = new Fl_Group(35, 120, 98, 60);

//        new Fl_Box(38, 160, 95, 20, "of the bed");
        o->end(); 
      }
      { Fl_Check_Button* o = wWrite = new Fl_Check_Button(30, 35, 75, 25, "Write");
        o->down_box(FL_DOWN_BOX);
		o->callback((Fl_Callback*)write_cb);
      }
      { Fl_Check_Button* o = wRead = new Fl_Check_Button(120, 35, 75, 25, "Read");
        o->down_box(FL_DOWN_BOX);
		o->callback((Fl_Callback*)read_cb);
      }
      { Fl_Check_Button* o = wVerify = new Fl_Check_Button(210, 35, 67, 25, "Verify");
        o->down_box(FL_DOWN_BOX);
		o->callback((Fl_Callback*)verify_cb);
      }
      o->end();
    }
    { Fl_Group* o = new Fl_Group(20, 160, 260, 100, "Programmer Options:");
      o->box(FL_ENGRAVED_FRAME);
      o->align(FL_ALIGN_TOP_LEFT);
      { Fl_Choice* o = wProgrammer = new Fl_Choice(30, 200, 105, 25, "Programmer:");
        o->down_box(FL_BORDER_BOX);
        o->align(FL_ALIGN_TOP_LEFT);
        o->menu(menu_wProgrammer);
	//	  o->callback((Fl_Callback*)beepcb);
      }
      { Fl_Choice* o = wPort = new Fl_Choice(165, 200, 105, 25, "Port:");
        o->down_box(FL_BORDER_BOX);
        o->align(FL_ALIGN_TOP_LEFT);
        o->menu(menu_wPort);
	//	  o->callback((Fl_Callback*)beepcb);
      }
        
      o->end();
    }
    o->end();
  }

if (just_opened==false)
{
  readPrefs();
just_opened=false;  
}
  

  
  w->show(argc, argv);
   Fl::run();
   }
}







void readPrefs() {
  int boolValue;

char buffer[80];
double doubleValue;


Fl_Preferences app( Fl_Preferences::USER, "fltk.org", "test/preferences" );

  char path[ FL_PATH_MAX ];
  app.getUserdataPath( path, sizeof(path) );

  Fl_Preferences chip( app, "Chip" );
 
    chip.get( "Chip", intValue, 1 );
    wChip->value( intValue );


    
	
	int tasks;
    chip.get( "taskFlags", tasks, 0x05 );
    if ( tasks & 0x01 )
	{
	 wWrite->value( 1 );
//	   write=true;
	 wf=true;
	 write_cb();
	 }
	 
    if ( tasks & 0x02 )
	{
	 wRead->value( 1 );
//	   read=true;
	 rf=true;
	 read_cb();
	 }
    if ( tasks & 0x04 )
	{
	 wVerify->value( 1 );
//	   verify=true;
	 vf=true;
	 verify_cb();
	 }
 Fl_Preferences pgmopt( app, "Programmer" );


 
   pgmopt.get( "programmer", intValue, 1 );
    wProgrammer->value( intValue );

    pgmopt.get( "wPort", intValue, 1 );
    wPort->value( intValue );

}

void writePrefs() {
  Fl_Preferences app( Fl_Preferences::USER, "fltk.org", "test/preferences" );

  Fl_Preferences chip( app, "Chip" );


    chip.set( "Chip", wChip->value() );



    int tasks = 0;
    if ( wWrite->value() ) tasks |= 0x01;
    if ( wRead->value() ) tasks |= 0x02;
    if ( wVerify->value() ) tasks |= 0x04;
    chip.set( "taskFlags", tasks );

  Fl_Preferences pgmopt( app, "Programmer" );
  
   
 pgmopt.set( "programmer", wProgrammer->value() );

   pgmopt.set( "port", wPort->value() );
   
}




void new_cb(Fl_Widget*, void*) {
  if (!check_save()) return;

  filename[0] = '\0';
  textbuf->select(0, textbuf->length());
  textbuf->remove_selection();
  changed = 0;
  textbuf->call_modify_callbacks();
}

void open_cb(Fl_Widget*, void*) {
  if (!check_save()) return;

  char *newfile = fl_file_chooser("Open File?", "*", filename);
  if (newfile != NULL) load_file(newfile, -1);
}

void insert_cb(Fl_Widget*, void *v) {
  char *newfile = fl_file_chooser("Insert File?", "*", filename);
  EditorWindow *w = (EditorWindow *)v;
  if (newfile != NULL) load_file(newfile, w->editor->insert_position());
}

void paste_cb(Fl_Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  Fl_Text_Editor::kf_paste(0, e->editor);
}

int num_windows = 0;

void close_cb(Fl_Widget*, void* v) {
  Fl_Window* w = (Fl_Window*)v;
  if (num_windows == 1 && !check_save()) {
    return;
  }

  w->hide();
  textbuf->remove_modify_callback(changed_cb, w);
  delete w;
  num_windows--;
  if (!num_windows) exit(0);
}

void quit_cb(Fl_Widget*, void*) {
  if (changed && !check_save())
    return;

  exit(0);
}

void replace_cb(Fl_Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  e->replace_dlg->show();
}

void replace2_cb(Fl_Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  const char *find = e->replace_find->value();
  const char *replace = e->replace_with->value();

  if (find[0] == '\0') {
    // Search string is blank; get a new one...
    e->replace_dlg->show();
    return;
  }

  e->replace_dlg->hide();

  int pos = e->editor->insert_position();
  int found = textbuf->search_forward(pos, find, &pos);

  if (found) {
    // Found a match; update the position and replace text...
    textbuf->select(pos, pos+strlen(find));
    textbuf->remove_selection();
    textbuf->insert(pos, replace);
    textbuf->select(pos, pos+strlen(replace));
    e->editor->insert_position(pos+strlen(replace));
    e->editor->show_insert_position();
  }
  else fl_alert("No occurrences of \'%s\' found!", find);
}

void replall_cb(Fl_Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  const char *find = e->replace_find->value();
  const char *replace = e->replace_with->value();

  find = e->replace_find->value();
  if (find[0] == '\0') {
    // Search string is blank; get a new one...
    e->replace_dlg->show();
    return;
  }

  e->replace_dlg->hide();

  e->editor->insert_position(0);
  int times = 0;

  // Loop through the whole string
  for (int found = 1; found;) {
    int pos = e->editor->insert_position();
    found = textbuf->search_forward(pos, find, &pos);

    if (found) {
      // Found a match; update the position and replace text...
      textbuf->select(pos, pos+strlen(find));
      textbuf->remove_selection();
      textbuf->insert(pos, replace);
      e->editor->insert_position(pos+strlen(replace));
      e->editor->show_insert_position();
      times++;
    }
  }

  if (times) fl_message("Replaced %d occurrences.", times);
  else fl_alert("No occurrences of \'%s\' found!", find);
}

void replcan_cb(Fl_Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  e->replace_dlg->hide();
}

void save_cb() {
  if (filename[0] == '\0') {
    // No filename - get one!
    saveas_cb();
    return;
  }
  else save_file(filename);
}

void saveas_cb(void) {
  char *newfile;

  newfile = fl_file_chooser("Save File As?", "*", filename);
  if (newfile != NULL) save_file(newfile);
}














	
	
	
	
	
Fl_Window* new_view();

void view_cb(Fl_Widget*, void*) {
  Fl_Window* w = new_view();
  w->show();
}

Fl_Menu_Item menuitems[] = {
  { "&File",              0, 0, 0, FL_SUBMENU },
    { "&New File",        0, (Fl_Callback *)new_cb },
    { "&Open File...",    FL_CTRL + 'o', (Fl_Callback *)open_cb },
    { "&Insert File...",  FL_CTRL + 'i', (Fl_Callback *)insert_cb, 0, FL_MENU_DIVIDER },
    { "&Save File",       FL_CTRL + 's', (Fl_Callback *)save_cb },
    { "Save File &As...", FL_CTRL + FL_SHIFT + 's', (Fl_Callback *)saveas_cb, 0, FL_MENU_DIVIDER },
    { "New &View", FL_ALT + 'v', (Fl_Callback *)view_cb, 0 },
    { "&Close View", FL_CTRL + 'w', (Fl_Callback *)close_cb, 0, FL_MENU_DIVIDER },
    { "E&xit", FL_CTRL + 'q', (Fl_Callback *)quit_cb, 0 },
    { 0 },

  { "&Edit", 0, 0, 0, FL_SUBMENU },
    { "Cu&t",        FL_CTRL + 'x', (Fl_Callback *)cut_cb },
    { "&Copy",       FL_CTRL + 'c', (Fl_Callback *)copy_cb },
    { "&Paste",      FL_CTRL + 'v', (Fl_Callback *)paste_cb },
    { "&Delete",     0, (Fl_Callback *)delete_cb },
    { 0 },

  { "&Search", 0, 0, 0, FL_SUBMENU },
    { "&Find...",       FL_CTRL + 'f', (Fl_Callback *)find_cb },
    { "F&ind Again",    FL_CTRL + 'g', find2_cb },
    { "&Replace...",    FL_CTRL + 'r', replace_cb },
    { "Re&place Again", FL_CTRL + 't', replace2_cb },
    { 0 },
	
  { "&Project", 0, 0, 0, FL_SUBMENU },
  	{"&Assemble", FL_F+5, assemble_cb },
	{"&Program Chip", FL_F+9,program_cb, 0, FL_MENU_DIVIDER },
	{"&Options",     0, (Fl_Callback *)options_cb },
	{ 0 },
  { "&Help", 0, 0, 0, FL_SUBMENU },
  	{ "&About", 0, about_cb },
	{ "&Manual", 0,(Fl_Callback *)help_cb },
	{ 0 },
  { 0 }
};

Fl_Window* new_view() {
  EditorWindow* w = new EditorWindow(660, 600, title);
    w->begin();
    Fl_Menu_Bar* m = new Fl_Menu_Bar(0, 0, 660, 30);
    m->copy(menuitems, w);
 
 
 //  file_edit = new Fl_Text_Editor(0, 40, w, 260, "File Editor");
   //     file_buff = new Fl_Text_Buffer();
    //    file_edit->buffer(file_buff);
     //   file_edit->textfont(FL_COURIER);

   //w->editor = new Fl_Text_Editor(0, 400, 655, 197,"Console Output");
      //  w->editor = new Fl_Text_Editor(0, 30, 655, 350);
   //   w->editor = new Fl_Text_Editor(0, 400, 655, 197,"Console Output");
    //w->editor->buffer(textbuf);
  
    w->editor = new Fl_Text_Editor(0, 45, 660, 340, "SAVR IDE");
    w->editor->buffer(textbuf);
	
        // Create console editor
        console_edit = new Fl_Text_Editor(0, 400, 655, 197, "Console Output");
        console_buff = new Fl_Text_Buffer();
        console_edit->buffer(console_buff);
        console_edit->textfont(FL_COURIER);

        // Popen stuff
        fp = NULL;
        command[0] = 0;

 
   
    w->editor->highlight_data(stylebuf, styletable,
                              sizeof(styletable) / sizeof(styletable[0]),
			      'A', style_unfinished_cb, 0);
    w->editor->textfont(FL_COURIER);
  w->end();
  w->resizable(w->editor);
  w->callback((Fl_Callback *)close_cb, w);

  textbuf->add_modify_callback(style_update, w->editor);
  textbuf->add_modify_callback(changed_cb, w);
  textbuf->call_modify_callbacks();
  num_windows++;
  return w;
}





int main(int argc, char **argv) {
  textbuf = new Fl_Text_Buffer;
  style_init();
//system("@echo Simple AVR IDE");
//  system("@echo (Do not close this window)");
just_opened=true;
  Fl_Window* window = new_view();

  window->show(1, argv);

  if (argc > 1) load_file(argv[1], -1);
/*   Fl_Window win(600,250,600,350);
    Fl_Multi_Browser brow(10,10,580,320);
//    if ( ( G_fp = popen(PING_CMD, "r") ) == NULL ) {
 //     perror("popen failed");
	//return(1);
    //}
Fl::add_fd(fileno(G_fp), HandleFD, (void*)&brow);

   
	win.resizable(brow);
    win.show();*/
  
    
  return Fl::run();
}

//
// End of "$Id: editor.cxx 4723 2005-12-30 10:13:17Z matt $".
//

