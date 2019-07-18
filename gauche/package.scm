;;
;; Package cp2130
;;

(define-gauche-package "cp2130"
  ;; 
  :version "1.0"

  ;; Description of the package.  The first line is used as a short
  ;; summary.
  :description "CP2130 USB-to-SPI Bridge binding"

  ;; List of dependencies.
  ;; Example:
  ;;     :require (("Gauche" (>= "0.9.5"))  ; requires Gauche 0.9.5 or later
  ;;               ("Gauche-gl" "0.6"))     ; and Gauche-gl 0.6
  :require ()

  ;; List of providing modules
  ;; NB: This will be recognized >= Gauche 0.9.7.
  ;; Example:
  ;;      :providing-modules (util.algorithm1 util.algorithm1.option)
  ;:providing-modules ()
  
  ;; List name and contact info of authors.
  ;; e.g. ("Eva Lu Ator <eval@example.com>"
  ;;       "Alyssa P. Hacker <lisper@example.com>")
  :authors ("skimu@me.com")

  ;; List name and contact info of package maintainers, if they differ
  ;; from authors.
  ;; e.g. ("Cy D. Fect <c@example.com>")
  :maintainers ()

  ;; List licenses
  ;; e.g. ("BSD")
  :licenses ()

  ;; Homepage URL, if any.
  :homepage "https://github.com/shkmr/cp2130"

  ;; Repository URL, e.g. github
  :repository "https://github.com/shkmr/cp2130.git"
  )
