; Andrew Berg Project 1 COP4020 10/18/2016

;provided in the write up
(define compile
	(lambda (filename)
		(begin (set-current-input-port! (open-input-file filename))
			(statement (read)))))
;provided in the write up
(define statement
	(lambda (token)
		(cond ((eq? token 'VAR) (var-statement))
		((eq? token 'DO)    (do-statement))
		((eq? token 'UNTIL) (until-statement))
		((eq? token 'FOR) (for-statement))
		((eq? token 'IF)  (if-statement))
		((eq? token 'LET) (let-statement))
		(else (expression token) ))))

;provided in the write up
(define expression
	(lambda (token)
		(cond ((number? token) token)
			  ((string? token) token)
			  ((pair? token) token)
			  ((eq? (string-ref (symbol->string token) 0) #\$) token)
			  ((eq? token 'TRUE) #t)
			  ((eq? token 'FALSE) #f)
			  (else (fun-expression token) )))) ;attaches the function name  to the fun-expression

;provided in the write up
;changed statements to (statements 'DONE)
(define var-statement
	(lambda ()
		(let* ( (d (declarations))
			   (s (statements 'DONE)))
			(cons 'let* (cons d s)))))

;provided in the write up
(define declaration
	(lambda (token)
		(list token (expression (read-after '=)))))


(define declarations
	(lambda () ; call the function with no arg
		(let ((head (read))) ; read into head
			(if (or (eq? head 'DO) (eq? head 'EOF)) '() ;check if 'DO or EOF
				(let ((head (declaration head))) ; run head through declaration
				(let ((tail (declarations))) ; recurse into tail
				(cons head tail))))))) ; combine head and tail to build the list

;provided in the write up
(define read-after
	(lambda (token)
	(if (eq? (read) token)
		(read)
		(display "Syntax error"))))

;provided in the write up
(define begin-statements
	(lambda (ending)
		(let ((s (statements ending)))
		(if (pair? s)
		    (cons 'begin s)
		 	'()))))

(define statements
	(lambda (ending) ; supplied ending of line from call
		(let ((head (read))) ; read into head from file
		(if (or (eq? head ending) (eof-object? head) ) '() ; 		   check for head == ending
			(let ((head (statement head))) ; run head through statement check for end of object
			(let ((tail (statements ending))) ; recurse to end of statement
			(cons head tail))))))) ; build the list with cons

; provided in the write up
(define run
	(lambda (filename)
	(let ((s-expr (compile filename)))
		(display s-expr)
		(newline)
		(eval s-expr user-initial-environment)
		(newline))))

(define fun-expression
	(lambda (token) ; takes in token from expression
	(let ((head token)) ; assigns token to head
		(if (or (eq? head '!)) '() ; check if head == !
		(let ((tail (statements '!))) ; run statements '! and store in tail
		(cons head tail)))))) ; recursively build the list

; DID I NOT REALIZE STATEMENTS WOULD FIX IT ALL
(define do-statement
	(lambda() ; no args
	(begin-statements 'DONE))) ; run begin statements till DONE

(define if-statement 
	(lambda ()
		(let* ((e (expression (read))) ; read the IF VALUE into e
		        (e1 (begin-statements 'ELSE)) ; read if true block
		    	(e2 (begin-statements 'ENDIF))) ; read else block
		(list 'if e e1 e2)))) ; build list based off these values

(define let-statement ; given by write up
	(lambda ()
	(let ((token (read)))
	(list 'set! token (expression (read-after '=))))))

(define until-statement
	(lambda () ; no args
	(let* ((e (expression (read))) ; read a value from file then into expression then to e
			(s (statements 'LOOP))) ; build s until 'LOOP is reached
	 (append (cons 'do (cons '() (list (list e '())))) s)))) ; some crazy stuff

(define for-statement ;reads fors
	(lambda ()
	(let* ((v (read)) ; read into v
			(e1 (read-after '=)) ; read into e1 after =
			(e2 (expression (read-after 'to))) ; evaluate the expression
			(s (statements 'LOOP))) ; read statements until LOOP
	(cons 'do (append (append (list (list (list v e1 (list '+ v 1)))) 
	  (list (list (list '> v e2) v))) s)) ))) ; some really crazy stuff


