#ifndef __H_CODE_MODEL__
#define __H_CODE_MODEL__

#include <memory>
#include <boost/bimap.hpp>
#include <QStringListModel>

#include <kraken/CodeListingInterface.h>

using namespace kraken;
using namespace boost::bimaps;

class CodeModel : public QStringListModel
{
    Q_OBJECT

  public:
    CodeModel(QString string, QObject* pobj);

    void showProgramListing();
    int getCodeBlockCount();
    int getCodeBlockItemCount();

    QModelIndex getFirstReferencedCode(QModelIndex index);

  private:
    typedef bimap<int, codeItemLocation_t> listingMap;
    typedef listingMap::value_type listingMapTuple;

    std::shared_ptr<CodeListingInterface> _codeListing;
    listingMap _listingToModelMap;

    QStringList getProgramListing();
    CodeModel() {}
};

#endif
