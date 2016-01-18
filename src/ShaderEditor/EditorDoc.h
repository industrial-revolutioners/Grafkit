
// ShaderEditorDoc.h : interface of the CEditorDoc class
//

#pragma once

// #include "EditorView.h"
#include "ShaderSrcDoc.h"

class CEditorView;

class CEditorDoc : public CDocument
{
	friend class CEditorView;

protected: // create from serialization only
	CEditorDoc();
	DECLARE_DYNCREATE(CEditorDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:

	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	CShaderSrcDoc  m_shader_src;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
