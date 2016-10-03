<?hh //strict

namespace Etsy\Crons;

use Plenty\Modules\Cron\Contracts\CronHandler;

use Etsy\Services\ItemExport;

class ItemExportCron extends CronHandler
{
    public function handle(ItemExport $service):void
    {
        $service->run();
    }
}