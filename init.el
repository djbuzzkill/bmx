;;
;;
(setq inhibit-startup-message t)

;;
(global-font-lock-mode t)
(show-paren-mode 1)
(scroll-bar-mode -1)
(menu-bar-mode -1)
(tool-bar-mode -1)


(transient-mark-mode 1)

(recentf-mode 1)

(save-place-mode 1)

(setq use-dialog-box nil)

(global-auto-revert-mode 1)

(global-display-line-numbers-mode t)

(set-fringe-mode 10)

(setq visible-bell t)

(require 'package)

(global-set-key (kbd "<escape>") 'keyboard-escape-quit)
(global-set-key (kbd "C-<tab>") 'other-window)
;;(global-set-key (kbd "C-m") 'set-mark-command)


;; quit putting Customize shit in this file
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

;; no line numbers for these buffers
(dolist (mode '(org-mode-hook
		term-mode-hook
                shell-mode-hook
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
	  
;;
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
  :init (load-theme 'doom-palenight t))

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
   "t" '(:ignore t :which-key "toggles")
   "tt" '(counsel-load-theme :which-key "choose theme")

   "b" '(:ignore b :which-key "buffer operations")
   "bb" '(counsel-ibuffer :which-key "switch buffer")
   

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


;;(global-set-key (kbd "C-m") 'set-mark-command)
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

(use-package org)

(use-package lsp-mode
  :commands (lsp lsp-deferred)
  :init (setq lsp-keymap-prefix "C-c l")
  :config (lsp-enable-which-key-integration))

;;
(use-package projectile
  :diminish projectile-mode
  :config (projectile-mode)
  :custom (projectile-completion-system 'ivy)
  :bind-keymap
  ("C-c p" . projectile-command-map)
  :init
  (when (file-directory-p "~/owens_lake")
    (setq projectile-project-search-path '("~/owens_lake")))
  (setq projectile-switch-project-action #'projectile-dired))
  
(use-package counsel-projectile
  :config (counsel-projectile-mode))

;;
(use-package magit
  :commands (magit-status magit-get-currennt-branch)
  :custom
  (magit-display-buffer-function #'magit-display-buffer-same-window-except-diff-v1))

;; evil collection 
;; (use-package evil-magit
;;   :after magit)


;;
(use-package python-mode
  :ensure t
  :custom (python-shell-interpreter "python3"))

;; 
(use-package flycheck
  :ensure t
  :init (global-flycheck-mode))

;; Common Lisp/Slime
(add-to-list 'load-path "~/owens_lake/slime")
(setq  inferior-lisp-program "~/bin/sbcl")
(require 'slime)
(slime-setup)
(slime-setup '(slime-fancy))

;; 
(setq initial-frame-alist '((width . 164) (height . 48) (x-pos . 0) (y-pos . 0)))
;;(setq iswitchb-mode t)



; (global-set-key "\M-o"  'find-file)
; (global-set-key "\M-g"  'goto-line)

(global-set-key  [(control tab)] 'next-multiframe-window)
;; other window


;; -- cycle buffers --
;;(define-key global-map (kbd "C-\,") 'previous-buffer)
;;(define-key global-map (kbd "C-\.") 'next-buffer)

;; 
;; (global-set-key "\M-s" 'slime-selector)

;;
(set-frame-parameter (selected-frame) 'alpha 90)

;; 
;; (find-file "c:/Quarantine/.emacs")
(find-file  "~/hello.org")
(find-file  "~/.emacs.d/init.el")
(find-file  "~/hello.lisp")

;;


;;


