#lang racket

(require scribble/text)
(require racket/match)
(require "utils.rkt")

(provide generate-header)
(provide generate-source)
(provide parse-field-declaration)

; --------------- member struct -----------------

(struct member (type id size))

(define (array? member)
   (>= (member-size member) 0))

(define (member->string member)
   (if (array? member)
      (~a "stdx::array<" (member-type member) "," (number->string (member-size member)) "> " (member-id member))
      (~a (member-type member) " " (member-id member))))

(define (non-native-type? member)
   (starts-uppercase? (member-type member)))

(define (native-type? member)
   (not (non-native-type? member)))

(define (sizeof-native-type member)
   (if (member? member)
      (~a "sizeof(" (member-id member) ")")
      (~a "sizeof(" member ")")))

(define (sizeof-user-type member)
   (if (member? member)
      (~a (member-id member) ".getSize()")
      (~a member ".getSize()")))

(define (sizeof member)
   (if (native-type? member)
      (sizeof-native-type member)
      (sizeof-user-type member)))

; --------------- code generation functions -----------------

(define (generate-header className members)
  (let ([messageDependenciesHeaders (include-dependencies members)]
        [memberVariables (declare-members members)]
        [defaultConstructor (generate-default-constructor className members)]
        [constructorParams (generate-members-as-params members)]
        [constructorParamsInit (generate-constructor-params-init members)]
        [sizeof (generate-sizeof members)]
        [serialization (generate-serialization members)]
        [deserialization (generate-deserialization members)])
    (include/text "template.h")))

(define (generate-source className members)
  (let ([constructorParams (generate-members-as-params members)]
        [constructorParamsInit (generate-constructor-params-init members)]
        [sizeof (generate-sizeof members)]
        [serialization (generate-serialization members)]
        [deserialization (generate-deserialization members)])
    (include/text "template.cpp")))

(define (parse-field-declaration str)
   (match str 
      [(regexp #rx"^([a-zA-Z_][a-zA-Z0-9_]*)\\[([0-9]+)\\] ([a-zA-Z_][a-zA-Z0-9_]*)$" (list _ type size id))
       (member type id (string->number size))]
      [(regexp #rx"^([a-zA-Z_][a-zA-Z0-9_]*) ([a-zA-Z_][a-zA-Z0-9_]*)" (list _ type id))
       (member type id -1)]
      [(regexp #rx".*")
       (display "Parsing failed!")]))

(define (generate-type-id-list members)
   (map member->string members))

(define (include-dependencies members)
   (define (include-type member)
      (~a "#include \"com/msg/" (member-type member) ".h\"\n"))
   (map include-type (filter non-native-type? members)))

(define (declare-members class-members)
   (define (transform str)
      (add-newline (add-semicolon str)))
   (map transform (generate-type-id-list class-members)))

(define (generate-members-as-params members)
   (string-join (generate-type-id-list members) ", "))

(define (generate-constructor-params-init members)
   (define (generate-member-initialization member)
      (~a (member-id member) "(" (member-id member) ")"))
   (if (empty? members)
      ""
      (~a ": " (string-join (map generate-member-initialization members) ", "))))

(define (generate-serialization members)
   (define (generate-field-serialization member)
      (~a "buffer = com::serialize(msg." (member-id member) ", buffer);\n"))
   (map generate-field-serialization members))

(define (generate-deserialization members)
   (define (generate-field-deserialization member)
      (~a "buffer = com::deserialize(buffer, msg." (member-id member) ");\n"))
   (map generate-field-deserialization members))

(define (size-tmp-var member)
   (~a (member-id member) "_size"))

(define (generate-sizeof-array member)
   (define sizeVar (size-tmp-var member))
   (define sizeOfArrayElement (if (native-type? member) 
                                 (sizeof-native-type "element")
                                 (sizeof-user-type "element")))
   (list (~a "uint32_t " sizeVar " = 0;\n")
         (~a "for (const auto& element : " (member-id member) ")\n")
         (~a "  " sizeVar " += " sizeOfArrayElement ";\n")))

(define (generate-sizeof-scalar member)
   (~a "uint32_t " (size-tmp-var member) " = " (sizeof member) ";\n"))

(define (generate-sizeof-member member)
   (if (array? member)
      (generate-sizeof-array member)
      (generate-sizeof-scalar member)))

(define (generate-sum-of-sizes members)
   (unless (empty? members)
      (~a "size += " (string-join (map size-tmp-var members) " + ") ";\n")))

(define (generate-sizeof members)
   (append (map generate-sizeof-member members) (generate-sum-of-sizes members)))

(define (generate-default-constructor className members)
   (if (empty? members)
      ""
      (~a className "() = default;")))
