#!@GUILE@ \
-e main -s
!#
;;;; lilypond-invoke-editor.scm -- Invoke an editor in file:line:column mode
;;;;
;;;; source file of the GNU LilyPond music typesetter
;;;;
;;;; (c)  2005 Jan Nieuwenhuizen <janneke@gnu.org>

(use-modules
 (ice-9 getopt-long)
 (ice-9 regex)
 (srfi srfi-13)
 (srfi srfi-14))

(define PROGRAM-NAME "lilypond-invoke-editor")
(define TOPLEVEL-VERSION "@TOPLEVEL_VERSION@")
(define DATADIR "@DATADIR@")
(define COMPILE-TIME-PREFIX
  (format #f "~a/lilypond/~a" DATADIR TOPLEVEL-VERSION))
(define LILYPONDPREFIX (or (getenv "LILYPONDPREFIX") COMPILE-TIME-PREFIX))

;; gettext wrapper for guile < 1.7.2
(if (defined? 'gettext)
    (define-public _ gettext)
    (define-public (_ x) x))

(define (show-version port)
  (format port "~a (GNU LilyPond) ~a \n" PROGRAM-NAME TOPLEVEL-VERSION))

(define (show-help port)
  (format port (_ "Usage: lilypond-invoke-editor [textedit://]FILE:LINE:COLUMN

Visit a file and position the cursor

Options:
  -h,--help          show this help
  -v,--version       show version
")))

(define (parse-options args)
  (let* ((options (getopt-long args
			       '((help (single-char #\h))
				 (version (single-char #\v)))))
	 (files (cdr (assq '() options))))
    (if (assq 'help options)
	(begin
	  (show-version (current-output-port))
	  (show-help (current-output-port))
	(exit 0)))
    (if (assq 'version options)
	(begin (show-version (current-output-port)) (exit 0)))
    (show-version (current-error-port))
    files))

(define (re-sub re sub string)
  (regexp-substitute/global #f re string 'pre sub 'post))

(define (dissect-uri uri)
  (let* ((ri "textedit://")
	 (file-name:line:column (re-sub ri "" uri))
	 (match (string-match "(.*):([^:]+):(.*)$" file-name:line:column)))
    (if match
	(list (match:substring match 1)
	      (match:substring match 2)
	      (match:substring match 3))
	(begin
	  ;; FIXME: why be so strict wrt :LINE:COLUMN,
	  ;; esp. considering omitting textedit:// is explicitly
	  ;; allowed.
	  (format (current-error-port) (_ "invalid URI: ~a") uri)
	  (newline (current-error-port))
	  (format (current-error-port) (_ "expect: ~aFILE:LINE:COLUMN") ri)
	  (newline (current-error-port))
	  (exit 1)))))

(define PLATFORM
  (string->symbol
   (string-downcase
    (car (string-tokenize (vector-ref (uname) 0) char-set:letter)))))

(define (running-from-gui?)
  (let ((have-tty? (isatty? (current-input-port))))
    ;; If no TTY and not using safe, assume running from GUI.
    ;; for mingw, the test must be inverted.
    (if (eq? PLATFORM 'windows)
	have-tty? (not have-tty?))))

(define (main args)
  (let ((files (parse-options args)))
    (if (running-from-gui?)
	(redirect-port (current-error-port)
		       (open-file (string-append
				   (or (getenv "TMP")
				       (getenv "TEMP")
				       "/tmp")
				   "/lilypond-invoke-editor.log") "a")))
    (if (not (= (length files) 1))
	(begin
	  (show-help (current-error-port))
	  (exit 1)))
    (set! %load-path (cons LILYPONDPREFIX %load-path))
    (primitive-eval '(use-modules (scm editor)))
    (let* ((uri (car files))
	   (command (apply get-editor-command (dissect-uri uri)))
	   (status (system command)))
      (if (not (= status 0))
	  (begin
	    (format (current-error-port)
		    (_ "failed to invoke editor: ~a") command)
	    (exit 1))))))
