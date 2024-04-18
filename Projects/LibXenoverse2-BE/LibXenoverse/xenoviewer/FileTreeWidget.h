#ifndef FILE_TREE_WIDGET_H_INCLUDED
#define FILE_TREE_WIDGET_H_INCLUDED

class FileTreeItemWidget;
class ModelPackItemWidget;
class SkeletonItemWidget;
class MaterialPackItemWidget;
class MaterialItemWidget;
class MaterialParameterItemWidget;
class BoneItemWidget;
class AnimationItemWidget;

class FileTreeWidget : public QTreeWidget 
{
	protected:
	public:
		FileTreeWidget(QWidget *parent);

		void dropEvent(QDropEvent *event);
		void dropItemOnItem(FileTreeItemWidget *source_item, FileTreeItemWidget *dest_item);

		//void keyPressEvent(QKeyEvent * event);
		void processContextMenuModelPack(ModelPackItemWidget *item, const QPoint& point);
		void processContextMenuMaterial(MaterialItemWidget *item, const QPoint& point);
		void processContextMenuSkeleton(SkeletonItemWidget *item, const QPoint& point);
		void processContextMenuBone(BoneItemWidget *item, const QPoint& point);
		void processContextMenuEmpty(const QPoint& point);
		void processContextMenu(const QPoint& point);
		void boneItemSelectionChanged(QTreeWidgetItem* item);
		
};

#endif