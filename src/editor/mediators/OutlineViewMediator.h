#pragma once 

#include "common.h"
#include "utils/Event.h"
#include "models/ViewModule.h"

class QAction;

namespace Idogep {

	class TreeItem;

	namespace Roles {

		class MediateOutlineViewRole : public ViewMediator {
		public:
			MediateOutlineViewRole();
			void MediateViews(View *parent, View *child);
			void MediateRoles(View *parent);

		public:
			void ItemSelectedEvent(TreeItem *item);
			Event<Grafkit::AnimationRef> onAnimationSelected;

		};

	}
}

