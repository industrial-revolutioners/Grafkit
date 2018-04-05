#pragma once

#include <QMainWindow>

#include "common.h"

#include "Event.h"

namespace Idogep {

	class Editor;
	class Document;

	class EditorApplication;

	class QGrafkitContextWidget;

	class CurveEditorWidget;
	class SceneGraphViewWidget;
	class LogWidget;

	class PropertyEditorWidget;
	//class AboutDialog;

	class CommandStack;

	class TreeItem;
	class TreeModel;

	// ---

	class ManageOutlineViewRole {
	public:
		ManageOutlineViewRole();
	public:
		void ItemSelectedEvent(TreeItem *item);
		Event<Grafkit::AnimationRef> onAnimationSelected;

	};

	class ManageCommandsRole {
	public:
		ManageCommandsRole();

		Event<> onUndo;
		Event<> onRedo;

		void CommandStackChangedEvent(CommandStack * const & stack);

	protected:
		virtual void ToggleUndo(bool enabled) = 0;
		virtual void ToggleRedo(bool enabled) = 0;

	protected:
		QAction * m_undoAct;
		QAction * m_redoAct;
	};

	class MainWindow : public QMainWindow, public ManageOutlineViewRole, public ManageCommandsRole {
	public:
		MainWindow(Editor * const & editor);

		// external events (called from outside)
		void DocumentChanged(Document * const & document);

		// internal events (called from inside)		
		Event<> onSave;

	protected:
		virtual void ToggleUndo(bool enabled);
		virtual void ToggleRedo(bool enabled);

	private:
		void closeEvent(QCloseEvent *event);

		private slots:
		void undoSlot() { onUndo(); }
		void redoSlot() { onRedo(); }
		void saveSlot() { onSave(); }

	private:
		void createActions();
		void createStatusBar();
		void createDockWindows();

		void connectEvents(Editor * const & editor);

		QMenu *m_viewMenu;

		CurveEditorWidget * m_curveEditor;
		SceneGraphViewWidget *m_outlineViewer;
		PropertyEditorWidget *m_propertyVewer;

		LogWidget* m_logWidget;
	};

}
