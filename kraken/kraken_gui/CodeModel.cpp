#include <QSet>

#include "CodeListingDummy.h"
#include "CodeModel.h"

using namespace std;
using namespace kraken;

CodeModel::CodeModel(QString pathToFile, QObject* pobj) : QStringListModel(pobj)
{
  _decompiler.set_target(pathToFile.toUtf8().constData());
  if( DP_ERROR == _decompiler.decompile() )
  {
    //TODO print message
  }

  _codeListing = _decompiler.low_level_listing();

  setStringList(getProgramListing());
}

void CodeModel::showProgramListing()
{
  setStringList(getProgramListing());
}

QStringList CodeModel::getProgramListing()
{
  QStringList appListing;

  _listingToModelMap.clear();
  if( false == _codeListing )
  {
    return appListing;
  }

  for ( int blockId = 0, blockCount = _codeListing->block_count(); blockId < blockCount; blockId++ )
  {
    appListing << QString( "Block #%1" ).arg( blockId );

    auto block = _codeListing->get_block_by_id(blockId);
    for ( int itemId = 0, itemCount = block->get_item_count(); itemId < itemCount; itemId++ )
    {
      auto item = _codeListing->get_item_by_id( blockId, itemId );

      appListing << QString::fromStdString( item->to_string() );
      _listingToModelMap.insert( codeItemLocationValue_t( appListing.count() - 1, codeItemLocation_t( blockId, itemId ) ) );
    }

    appListing << QString( "" );
  }

  return appListing;
}

CodeBlockItemInterface* CodeModel::getCodeItemByModelIndex(const QModelIndex& index) const
{
  auto locationIter = _listingToModelMap.left.find( index.row() );

  auto blockId = locationIter->second.get<0>();
  auto indexId = locationIter->second.get<1>();

  if ( blockId < 0 || indexId < 0 )
  {
    return NULL;
  }

  return _codeListing->get_item_by_id( blockId, indexId );
}

QModelIndex CodeModel::getModelIndexByCodeItemLocation(const codeItemLocation_t& codeItemLocation) const
{
  auto viewIndex = _listingToModelMap.right.find( codeItemLocation_t( codeItemLocation.get<0>(), codeItemLocation.get<1>() ) )->second;
  return index( viewIndex );
}

int CodeModel::getCodeBlockCount() const
{
  return _codeListing->block_count();
}

int CodeModel::getCodeBlockItemCount() const
{
  return _codeListing->item_count();
}

bool CodeModel::isCodeItemBranchType(const QModelIndex& index) const
{
  auto codeItem = getCodeItemByModelIndex( index );

  return codeItem == NULL ? false : codeItem->is_branch();
}

QModelIndex CodeModel::getFirstReferencedCodeItemIndex(const QModelIndex& modelIndex) const
{
  if (!modelIndex.isValid())
  {
    return modelIndex;
  }

  auto codeItem = getCodeItemByModelIndex( modelIndex );

  if ( codeItem == NULL || !codeItem->is_branch() )
  {
    return modelIndex;
  }

  return getModelIndexByCodeItemLocation( codeItem->go_to().front() );
}
