;;
;;
;;
;;


(setq inhibit-startup-message t)

;;
(global-font-lock-mode t)
(show-paren-mode 1)
(scroll-bar-mode -1)
(menu-bar-mode -1)
(tool-bar-mode -1)
(toggle-truncate-lines 1)

(transient-mark-mode 1)

(recentf-mode 1)

(save-place-mode 1)

(setq use-dialog-box nil)

(global-auto-revert-mode 1)

(global-display-line-numbers-mode t)

(set-fringe-mode 10)

(setq visible-bell nil)

(require 'package)

(global-set-key (kbd "<escape>") 'keyboard-escape-quit)
(global-set-key (kbd "C-<tab>") 'other-window)

(global-set-key (kbd "M-m") 'set-mark-command)
(global-set-key (kbd "M-j") 'forward-line)
(global-set-key (kbd "M-k") 'previous-line)
(global-set-key (kbd "M-n") 'forward-line)
(global-set-key (kbd "M-p") 'previous-line)

(global-unset-key (kbd "C-<backspace>"))


;; quit putting Customize shit in this filk
(setq custom-file (locate-user-emacs-file "custom_vars.el"))
(load custom-file 'noerror 'nomessage)


;; package places
(setq package-archives '(("melpa" . "https://melpa.org/packages/")
			 ("melpa-stable" . "https://stable.melpa.org/packages/")
			 ("org" . "https://orgmode.org/elpa/")
			 ("elpa" . "https://elpa.gnu.org/packages/")))

(package-initialize)
;;
(unless package-archive-contents
  (package-refresh-contents))

;; initialize use-package on non linux platforms
(unless (package-installed-p 'use-package)
  (package-install 'use-package))


(require 'use-package)


	 
(setq use-package-always-ensure t)

;;; https://github.com/Alexander-Miller/treemacs/issues/164
(with-eval-after-load 'treemacs

  (defun treemacs-ignore-example (filename absolute-path)
    (or (string-match "/Blender" absolute-path)
	(string-match "/Bullet_platform" absolute-path)
	(string-match "/OpenGL_system" absolute-path)
	(string-match "/Rn_" absolute-path)
	(string-match "/ninjabuild" absolute-path)
	(string-match "/Debug" absolute-path)
	(string-match "/Release" absolute-path)

	(string-match "/shader" absolute-path)
	(string-match "/spux" absolute-path)
	(string-match "/spux_SDL2" absolute-path)
	(string-match "/res" absolute-path)
	(string-match "/SDL2_platform" absolute-path)
	(string-match "/Hx" absolute-path)
	(string-match "/Ma_" absolute-path)
	(string-match "Terrain_renderer" absolute-path)
	(string-match "Ma_" absolute-path)

	(string-match "/test_Mars" absolute-path)
	(string-match "/data" absolute-path)
	(string-match "/Charon" absolute-path)
	(string-match "/Dx" absolute-path)))

  (add-to-list 'treemacs-ignored-file-predicates #'treemacs-ignore-example))



; no line numbers for these buffers
(dolist (mode '(org-mode-hook
		term-mode-hook
                shell-mode-hook
		treemacs-mode-hook
		eshell-mode-hook))
  (add-hook mode (lambda  () (display-line-numbers-mode 0))))
  

;;
;;
(use-package command-log-mode)

;;
;; better mode buffer functionality
(use-package ivy
  :diminish
  :bind (("C-s" . swiper)
	 :map ivy-minibuffer-map
	 ("TAB" . ivy-alt-done)
	 ("C-l" . ivy-alt-done)
	 ("C-j" . ivy-next-line)
	 ("C-k" . ivy-previous-line)
	 :map ivy-switch-buffer-map
	 ("C-k" . ivy-previous-line)
	 ("C-l" . ivy-done)
	 ("C-d" . ivy-switch-buffer-kill)
	 :map ivy-reverse-i-search-map
	 ("C-k" . ivy-previous-line)
	 ("C-d" . ivy-reverse-i-search-kill))
  :config
  (ivy-mode 1))
	  

;; remember to "install fonts"
(use-package all-the-icons)



;;
(global-set-key (kbd "C-M-j") 'counsel-switch-buffer)

;; (define-key emacs-lisp-mode-map (kbd "C-d u") 'counsel-load-theme)

;; DOOM ;;
(use-package doom-modeline
  :ensure t
  :init (doom-modeline-mode 1)
  :custom ((doom-modeline-height 16)))

;; 
(use-package doom-themes
  :init (load-theme 'doom-dark+ t))

(use-package rainbow-delimiters
  :hook (prog-mode . rainbow-delimiters-mode))


;; wtf is this for
(use-package which-key
  :init (which-key-mode)
  :diminish which-key-mode
  :config (setq which-key-idle-delay 0.3))


;; 
(use-package ivy-rich
  :init (ivy-rich-mode 1))


;; 
(use-package counsel
  :ensure t
  :bind (("M-x" . counsel-M-x)
	 ("C-x b" . counsel-ibuffer)
	 ("C-x C-f" . counsel-find-file)
	 :map minibuffer-local-map
	 ("C-r" . 'counsel-minibuffer-history)))


;
(use-package helpful
  :custom
  (counsel-describe-function-function #'helpful-callable)
  (counsel-describe-variable-function #'helpful-variable)
  :bind
  ([remap describe-function] . counsel-describe-function)
  ([remap describe-command] . helpful-command)
  ([remap describe-varialbe] . counsel-describe-variable)
  ([remap describe-key] . helpful-key))

;; 
(use-package general
  :config
  (general-create-definer e9/leader-keys
    :keymaps '(normal insert visual emacs)
    :prefix "SPC"
    :global-prefix "C-SPC")
  (e9/leader-keys
   "t"  '(:ignore t :which-key "toggles")
   "tt" '(counsel-load-theme :which-key "choose theme")

   "b"  '(:ignore b :which-key "buffer operations")
   "bb" '(counsel-ibuffer :which-key "switch buffer")

   "f"  '(:ignore f :which-key "file operations")
   "fp" '(find-file-at-point :which-key "open file at point")
   "fr" '(recentf-open-files :which-key "open recent file list")
   "fm" '(recentf-open-more-files :which-key "open more recent files")
   ))


;; vi in emacs
(use-package evil
  :init (setq evil-want-integration t)
        (setq evil-want-keybinding nil)
        (setq evil-want-C-u-scroll t)
	(setq evil-want-C-i-jump nil)
  :config
  (evil-mode 1)
  (define-key evil-insert-state-map (kbd "C-g") 'evil-normal-state)
  (define-key evil-insert-state-map (kbd "C-h") 'evil-delete-backward-char-and-join)
  (define-key evil-insert-state-map (kbd "C-d") 'evil-delete-char) 

  (evil-global-set-key 'motion "j" 'evil-next-visual-line)
  (evil-global-set-key 'motion "k" 'evil-previous-visual-line)

  (evil-set-initial-state '*ielm* 'normal)
  (evil-set-initial-state 'messages-buffer-mode 'normal)
  (evil-set-initial-state 'dashboard-mode 'normal))


;; 
(use-package evil-collection
  :after evil
  :config (evil-collection-init))
	  ;;

;; comment/uncomment code
(use-package evil-nerd-commenter
  :bind ("M-/" . evilnc-comment-or-uncomment-lines))


;;
(use-package dired
  :ensure nil
  :commands (dired dired-jump)
  :bind (("C-x C-j" . dired-jump))
  :custom (dired-listing-switches "-agho --group-directories-first")
  :config
  (evil-collection-define-key 'normal 'dired-mode-map
    "h" 'dired-up-directory
    "l" 'dired-find-file))



;; for keybinding stuff
(use-package hydra)

(defhydra hydra-text-scale (:timeout 4)
  "scale text"
  ("j" text-scale-increase "in")
  ("k" text-scale-decrease "out")
  ("f" nil "finished" :exit t))
;; wah :w

(e9/leader-keys
 "ts" '(hydra-text-scale/body :which-key "scale-text"))
  
(use-package forge)



(defun e9/org-mode-setup ()
  (org-indent-mode)
  (variable-pitch-mode 1)
  (auto-fill-mode 0)
  (visual-line-mode 1)
  (setq evil-auto-indent nil))


;;
(use-package org
;; :hook (org-mode . 
  :config (setq org-ellipsis " ▾"))



(use-package org-bullets
  :after org
  :hook (org-mode . org-bullets-mode)
  :custom (org-bullets-bullet-list '("◉" "○" "●" "○" "●" "○" "●")))
;; Α α, Β β, Γ γ, Δ δ, Ε ε, Ζ ζ, Η η, Θ θ, Ι ι, Κ κ, Λ λ, Μ μ, Ν ν, Ξ ξ, Ο ο, Π π, Ρ ρ, Σ σ/ς, Τ τ, Υ υ, Φ φ, Χ χ, Ψ ψ, Ω ω.





(defun e9/lsp-mode-setup ()
  (setq lsp-headerline-breadcrumb-segments '(path-up-to-project file symbols))
  (lsp-headerline-breadcrumb-mode))


(use-package lsp-mode
  :commands (lsp lsp-deferred)
  :hook (lsp-mode . e9/lsp-mode-setup)

  :init (setq lsp-keymap-prefix "C-c l")
  :config (lsp-enable-which-key-integration t))


;; 
(use-package lsp-ui
  :hook (lsp-mode . lsp-ui)
  :custom (lsp-ui-doc-position 'bottom))


;;
(use-package lsp-treemacs
  :after lsp)





;;
(use-package dap-mode)

;; https://github.com/emacs-lsp/dap-mode/issues/442
(require 'dap-gdb-lldb)


;; ?? is this needed ??
;; (require 'dap-node)

;;
(use-package company
  :after lsp-mode
  :hook (lsp-mode . company-mode)
  :bind
  (:map company-active-map
	("<tab>" . company-complete-selection))
  (:map lsp-mode-map
	("<tab>" . company-indent-or-complete-common))
  :custom
  (company-minimum-prefix-length 2)
  (company-idle-delay 0.0))



;;(use-package company-box
;; :hook (company-mode . company-box-mode))


;;
(use-package lsp-ivy)




;; (add-to-list 'auto-mode-alist '("\\.h\\'" . cc-mode))

;;
(use-package projectile
  :diminish projectile-mode
  :config (projectile-mode)
  :custom (projectile-completion-system 'ivy)
  :bind-keymap
  ("C-c p" . projectile-command-map)
  :init
  (setq projectile-project-search-path '(("~/owenslake" . 2) ("~/saltonsea" . 2)))
  (setq projectile-switch-project-action #'projectile-dired))

;; 

;;
;; (use-package treemacs-projectile


;;   
(use-package counsel-projectile
  :config (counsel-projectile-mode))

;;
(use-package magit
  :commands (magit-status magit-get-currennt-branch)
;;  :init
;;  (bind-key "C-x g" 'magit-status)
  :custom
  (magit-display-buffer-function #'magit-display-buffer-same-window-except-diff-v1))

;;
;; evil collection
 (use-package evil-collection
   :after magit)

;;
;;
(use-package term
  :config
  (setq explicit-shell-filename "bash")
  (setq term-prompt-regexp "^[^#$%>\n]*[#$%>] *"))
  
;;
(use-package eterm-256color
  :hook (term-mode . eterm-256color-mode))

(use-package vterm
  :commands vterm

  :config
  (setq vterm-max-scrollback 10000))



;;
(use-package eshell-git-prompt)



(defun e9/configure-eshell ()
  (add-hook 'eshell-pre-command-hook 'eshell-truncate-buffer)

  (add-to-list 'eshell-output-filter-functions 'eshell-truncate-buffer)

  (evil-define-key '(normal insert visual) eshell-mode-map (kbd "C-r") 'counsel-esh-history)
  (evil-define-key '(mormal insert visual) eshell-mode-map (kbd "<home>") 'eshell-bol)
  (evil-normalize-maps)

  (setq eshell-history-size (* 10 1024)
	eshell-buffer-maximum-lines  (* 10 1024)
	eshell-hist-ignoredupes t
	eshell-scroll-to-bottom-on-input t))


;;

(use-package eshell
  :hook (eshell-first-time-mode . 'e9/configure-eshell)
  :config (eshell-git-prompt-use-theme 'robbyrussell))




;;(use-package cc-mode
;;  :mode "\\.cpp\\'")

;;
(use-package python-mode
  :ensure t
  :hook (python-mode . lsp-deferred)
  :custom
  (python-shell-interpreter "python3")
  (dap-python-executable "python3")
  (dap-python-debugger 'debugpy)
  :config
  (require 'dap-python))

;;
(use-package pyvenv
  :config (pyvenv-mode 1))

;; 
(use-package flycheck
  :ensure t
  :init (global-flycheck-mode t))


;; (use-package dirvish
;;   :ensure t
;;   :init
;;   ;; Let Dirvish take over Dired globally
;;   (dirvish-override-dired-mode))

;;
;(require 'slime)
;; (use-package slime
;;         (slime-setup)
;;   :config (slime-setup '(slime-fancy)))
(use-package sly
  :init (setq inferior-lisp-program "/usr/bin/sbcl")

  :config (setq lisp-mode-hook 'sly-editing-mode))  

 	
;;(require 'sly-autoloads)



(eval-after-load 'sly
  `(define-key sly-prefix-map (kbd "M-h") 'sly-documentation-lookup))

;; 
(setq initial-frame-alist '((width . 164) (height . 48) (x-pos . 0) (y-pos . 0)))
;;(setq iswitchb-mode 



; (global-set-key "\M-o"  'find-file)
; (global-set-key "\M-g"  'goto-line)

(global-set-key  [(control tab)] 'next-multiframe-window)
;; other window


;; -- cycle buffers --
;;(define-key global-map (kbd "C-\,") 'previous-buffer)
;;(define-key global-map (kbd "C-\.") 'next-buffer)


;;
(set-frame-parameter (selected-frame) 'alpha 92)

;; 
;; (find-file "c:/Quarantine/.emacs")
(find-file  "~/hello.lisp")
(find-file  "~/hello.org")
(find-file  "~/.emacs.d/init.el")


;;


