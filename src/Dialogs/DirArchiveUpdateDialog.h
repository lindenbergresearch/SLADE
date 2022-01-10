
#ifndef __DIR_ARCHIVE_UPDATE_DIALOG__
#define __DIR_ARCHIVE_UPDATE_DIALOG__

#include "Archive/Formats/DirArchive.h"
#include "UI/SDialog.h"


class wxDataViewListCtrl;


class DirArchiveUpdateDialog : public SDialog {
public:
    DirArchiveUpdateDialog(wxWindow *parent, DirArchive *archive, vector<DirEntryChange> &changes);


    ~DirArchiveUpdateDialog();


    void populateChangeList();


    // Events
    void onBtnOKClicked(wxCommandEvent &e);


private:
    DirArchive *archive_;
    vector<DirEntryChange> changes_;
    wxDataViewListCtrl *list_changes_;
};


#endif//__DIR_ARCHIVE_UPDATE_DIALOG__
