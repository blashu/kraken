#include <QSet>

#include "CodeListingDummy.h"
#include "CodeModel.h"

using namespace std;

CodeModel::CodeModel(QString pathToFile, QObject* pobj) : QStringListModel(pobj)
{
  _codeListing = std::shared_ptr<CodeListingInterface>(new CodeListingDummy());

  setStringList(getProgramListing());
}

void CodeModel::showProgramListing()
{
  setStringList(getProgramListing());
}

QStringList CodeModel::getProgramListing()
{
  _listingToModelMap.clear();

  QStringList appListing;

  for ( int blockId = 0, blockCount = _codeListing->block_count(); blockId < blockCount; blockId++ )
  {
    appListing << QString( "Block #%1" ).arg( blockId );

    for ( int itemId = 0, itemCount = _codeListing->item_count(); itemId < itemCount; itemId++ )
    {
      auto item = _codeListing->get_item_by_id( blockId, itemId );
      appListing << QString::fromStdString( item->to_string() );

      _listingToModelMap.insert( listingMapTuple( appListing.count() - 1, codeItemLocation_t( blockId, itemId ) ) );
    }

    appListing << QString( "" );
  }

  return appListing;
}

int CodeModel::getCodeBlockCount()
{
  return _codeListing->block_count();
}

int CodeModel::getCodeBlockItemCount()
{
  return _codeListing->item_count();
}

QModelIndex CodeModel::getFirstReferencedCode(QModelIndex modelIndex)
{
  auto locationIter = _listingToModelMap.left.find( modelIndex.row() );

  auto codeItem = _codeListing->get_item_by_id( locationIter->second.get<0>(), locationIter->second.get<1>() );

  auto firstReferenceLocation = codeItem->go_to().front();

  auto firstReferenceViewIndex = _listingToModelMap.right.find( codeItemLocation_t( firstReferenceLocation.get<0>(), firstReferenceLocation.get<1>() ) )->second;

  return index( firstReferenceViewIndex );
}
