<?hh //strict

namespace Etsy\Services;

use Plenty\Modules\Item\DataLayer\Models\RecordList;
use Plenty\Modules\Item\DataLayer\Models\Record;
use Plenty\Modules\Item\DataLayer\Contracts\ItemDataLayerRepositoryContract;
use Plenty\Plugin\ConfigRepository;
use Etsy\Api\Client;

class ItemExport
{	
    private ItemDataLayerRepositoryContract $itemDataLayerRepository;
    private Client $client;
    private ConfigRepository $config;
    
	public function __construct(ItemDataLayerRepositoryContract $itemDataLayerRepository, Client $client, ConfigRepository $config)
	{            
        $this->itemDataLayerRepository = $itemDataLayerRepository;
        $this->client = $client;
        $this->config = $config;
	}

    public function run():void
    {     
        $records = $this->getRecords();

        foreach($records as $item)
        {
        	$this->startListing($item);
        }        
    }

    /**
     * Get items using the data layer.
     * 
     * @return RecordList
     */
    private function getRecords():RecordList
    {
        $columns = [
            'itemBase' => [
                'producer',
            ],

            'itemDescription' => [
                'params' => [
                    'language' => 'de',
                ],
                'fields' => [
                    'name1',
                    'description',
                    'shortDescription',
                    'technicalData',
                    'keywords'
                ],
            ],                        

            'variationRetailPrice' => [
                'price',
            ],

            'variationStock' => [
                'params' => [
                    'type' => 'virtual'
                ],
                'fields' => [
                    'stockNet'
                ]
            ],
        ];

        $filter = [
            'variationBase.isActive?' => [],
            'variationVisibility.isVisibleForMarketplace' => [
                'mandatoryOneMarketplace' => [],
                'mandatoryAllMarketplace' => [
                    $this->config->get('EtsyIntegrationShowcasePlugin.referrerId')
                ]
            ]
        ];

        return $this->itemDataLayerRepository->search($columns, $filter);
    }

	/**
	 * Start a listing.
     * 
	 * @param Record $item
	 * @return void
	 */
    private function startListing(Record $item):void
    {    	
        $data = [
            'quantity'              => $item->variationStock->stockNet,
            'title'                 => $item->itemDescription->name1,
            'description'           => $item->itemDescription->description,
            'price'                 => $item->variationRetailPrice->price,
            'shipping_template_id'  => 28734983909,
            'state'                 => 'draft',
            'who_made'              => 'i_did',
            'is_supply'             => 'false',
            'when_made'             => 'made_to_order'            
        ];

        $response = $this->client->call(
            'createListing', 
            ['language' => 'de'], 
            $data
        );
    }    
}