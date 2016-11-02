/*
 * Created by Florian Märkl <info@florianmaerkl.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PSVR_PSVR_TASK_H
#define PSVR_PSVR_TASK_H

#include <QThread>
#include "psvr.h"

class PSVRThread : public QThread
{
	Q_OBJECT

	private:
		PSVR *psvr;

	public:
		PSVRThread(PSVR *psvr, QObject *parent = 0);
		~PSVRThread();

	signals:
		void PSVRUpdate();

	protected:
		void run() Q_DECL_OVERRIDE;
};

#endif //PSVR_PSVR_TASK_H
