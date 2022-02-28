; I could only managed to tokenize single character terminals

(defun checkChar (line)
    (cond 
        ((string= line '+)
            (format t "OP_PLUS~%"))

        ((string= line '-)
            (format t "OP_MINUS~%"))

        ((string= line '/)
            (format t "OP_DIV~%"))
        
        ((string= line '*)
            (format t "OP_MULT~%"))

        ((string= line '\( )
            (format t "OP_OP~%"))

        ((string= line '\) )
            (format t "OP_CP~%"))

        ((string= line "**" )
            (format t "OP_DBLMULT~%"))

       ;; ((string= line '\")
         ;;   (format t "OP_OC~%"))

        ((string= line '\, )
            (format t "OP_COMMA~%"))
        
        ))


(defun gppinterpreter (filename)
(let ((in (open filename :if-does-not-exist nil)))
  (when in
    (loop for line = (read-char in nil)
        while line do 
        (checkChar line)
    )
    (close in)))
)

(defvar filename (read))
(gppinterpreter filename)